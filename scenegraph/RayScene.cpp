#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"

#include <iostream>
#include <limits>
#include <algorithm>

#include "glm/gtx/transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "util/MyMath.h"

#include "scenegraph/RayCylinder.h"
#include "scenegraph/RaySphere.h"
#include "scenegraph/RayCube.h"
#include "scenegraph/RayCone.h"
#include "scenegraph/Constants.h"



RayScene::RayScene(Scene &scene) :
    Scene(scene)
{

    // The comments say I might want to have texture IDs here.
    // A default scene might not contain textures.
    // TODO [INTERSECT]
    // Remember that any pointers or OpenGL objects (e.g. texture IDs) will
    // be deleted when the old scene is deleted (assuming you are managing
    // all your memory properly to prevent memory leaks).  As a result, you
    // may need to re-allocate some things here.
}


void RayScene::render(CS123SceneCameraData *camera, int width, int height, Canvas2D* canvas) {

    width = canvas->width();
    height = canvas->height();

    glm::vec4 eye = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // in camera space


    glm::vec4 w = - glm::normalize(camera->look);

    glm::vec4 v = camera->up - glm::dot( camera->up, w ) * w;

    v = glm::normalize(v);

    glm::vec4 u = glm::vec4( glm::cross( glm::vec3(v.x, v.y, v.z), glm::vec3(w.x, w.y, w.z) ), 0.0f );

    assert(glm::dot(u, w) == 0.0f);
    assert(glm::dot(u, v) == 0.0f);

    glm::mat4 worldTranslation = glm::mat4(1.0f, 0.0f, 0.0f, -camera->pos.x,
                                           0.0f, 1.0f, 0.0f, -camera->pos.y,
                                           0.0f, 0.0f, 1.0f, -camera->pos.z,
                                           0.0f, 0.0f, 0.0f, 1.0f);


    glm::mat4 worldRotation = glm::mat4(u.x, u.y, u.z, 0.0f,
                                        v.x, v.y, v.z, 0.0f,
                                        w.x, w.y, w.z, 0.0f,
                                        0.0f,0.0f,0.0f,1.0f);


    glm::mat4 cameraToWorld = glm::inverse(worldRotation * worldTranslation);


    RGBA *pix = canvas->data();


    for (int j = 0; j < height; j++) { // change this so it can vary the number of rays traced per pixel (and support fraction eg. 0.5 rays traced per pixel)
        for (int i = 0; i < width; i++) { // for each sample of image

            if ((width * j) + i < (canvas->width() * canvas->height())  && height > 0) {

                float x_notCenter = (static_cast<float>(i) + 0.5f) / static_cast<float>(width); // range [0, 1] "in unit sq"
                float y_notCenter = (static_cast<float>(j) + 0.5f) / static_cast<float>(height);

                float x = ( x_notCenter  ) - 0.5f;
                float y = 0.5f - ( y_notCenter );

                float k = 1.0f; // changing k should not affect anything.
                float theta_h = glm::radians(camera->heightAngle);
                float theta_w = glm::atan( (width/height) * glm::tan(theta_h) );

                float U = 2.0f * k * glm::tan(theta_w/2.0f);
                float V = 2.0f * k * glm::tan(theta_h/2.0f);

                glm::vec4 uvk = glm::vec4( U*x, V*y, -k, 1.0f ); // scaling back to width and height dimensions
                glm::vec4 d = glm::normalize(uvk - eye); // d in camera space


                d = MyMath::multiply(cameraToWorld, d); // d in world space
                pix[(width * j) + i] = trace(camera->pos, d);

            } else {
                std::cout << "ERROR indexing to " << width * j + i
                          << " pixel array is of smaller size " << canvas->width() * canvas->height() << std::endl;
            }
        }
    }
    canvas->update();
}


RGBA RayScene::trace(glm::vec4 p, glm::vec4 d) { // world space eye & dir

    float t = std::numeric_limits<float>::infinity();
    RGBA finalColor = RGBA(0.0f, 0.0f, 0.0f, 255.0f);

    for (SceneObject *obj : m_objects) {

        glm::vec4 pObj = glm::inverse(obj->cMTM) * p; // transforming to object space;
        glm::vec4 dObj = glm::inverse(obj->cMTM) * d;

        if (!(intersects(pObj, dObj, obj) == 0)  && (intersects(pObj, dObj, obj) < t)) {

            t = intersects(pObj, dObj, obj);

            glm::vec4 surfacePoint = glm::vec4( pObj.x + dObj.x * t, // in object space
                                                pObj.y + dObj.y * t,
                                                pObj.z + dObj.z * t,
                                                1.0f);

            glm::vec4 normal = getNormal(surfacePoint, obj); // in object space

            glm::mat3 m_3 = glm::inverse(glm::transpose(   glm::mat3(obj->cMTM)   ));

            glm::mat4 normalToWorld = glm::mat4(m_3);

            glm::vec4 normalWrld = normalToWorld * normal; // in world space

            surfacePoint = glm::inverse(obj->cMTM) * surfacePoint;

            finalColor = finalIntensity(obj->primitive->material.cAmbient,
                                        obj->primitive->material.cDiffuse,
                                        surfacePoint, normalWrld);
        }

    }

   return finalColor;
}


/**
 * Object normal must be in world space.
 */
RGBA RayScene::finalIntensity(glm::vec4 objAmbient, glm::vec4 objDiffuse, glm::vec4 point, glm::vec4 normal) {


    float ambient_r = m_global->ka * objAmbient.x; // Assuming vals [0, 1]
    float ambient_g = m_global->ka * objAmbient.y; // Assuming vals [0, 1]
    float ambient_b = m_global->ka * objAmbient.z; // Assuming vals [0, 1]

    float diffuse_r = 0.0f;
    float diffuse_g = 0.0f;
    float diffuse_b = 0.0f;

    float dummy = 1.01f;

    float nDotL;
    glm::vec4 incomingLightStep;

    for (CS123SceneLightData &light: m_lights) {
        glm::vec4 incomingLight = - glm::normalize(light.pos - point);

        incomingLightStep = incomingLight;

        nDotL = glm::dot(incomingLight, normal);

        float thisDiffuse_r = light.color.x * (m_global->kd * objDiffuse.x *
                                             nDotL);

        dummy = dummy + 1.0f;
        diffuse_r = diffuse_r + thisDiffuse_r;

        diffuse_g = diffuse_g + light.color.y * (m_global->kd * objDiffuse.y *
                                                 nDotL );

        diffuse_b = diffuse_b + light.color.z * (m_global->kd * objDiffuse.z *
                                                 nDotL );

//        diffuse_b = diffuse_b + light.color.z * (m_global->kd * objDiffuse.z *
//                                                 ((objNormal.z + 1.0f) /2.0f) );
    }


    //diffuse_r = 0.70710688829422f;

    RGBA finalColor = RGBA(

                255 * (ambient_r + diffuse_r) ,

                255 * (ambient_g + diffuse_g) ,

                255 * (ambient_b + diffuse_b) ,

                255
                );


//    RGBA finalColor = RGBA(255 * (incomingLightStep.x + 1.0f) /2.0f, //r = x
//                           255 * (incomingLightStep.y + 1.0f) /2.0f, //g = y
//                           255 * (incomingLightStep.z + 1.0f) /2.0f, //b = z
//                           255
//                           );

    return finalColor;
}


glm::vec4 RayScene::getNormal(glm::vec4 p, SceneObject *object) { // in world space eye & dir

    switch(object->primitive->type) {
        case PrimitiveType::PRIMITIVE_CONE:
            return RayCone::getNormal(p);
            break;
        case PrimitiveType::PRIMITIVE_CUBE:
            return RayCube::getNormal(p);
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            return RayCylinder::getNormal(p);
            break;
        case PrimitiveType::PRIMITIVE_TORUS:

            break;
        case PrimitiveType::PRIMITIVE_SPHERE:
            return RaySphere::getNormal(p);
            break;
        case PrimitiveType::PRIMITIVE_MESH:

            break;
    }

    return glm::vec4(0.0f);
}

float RayScene::intersects(glm::vec4 p, glm::vec4 d, SceneObject *object) { // in world space eye & dir

    switch(object->primitive->type) {
        case PrimitiveType::PRIMITIVE_CONE:
            return RayCone::intersects(p, d);
            break;
        case PrimitiveType::PRIMITIVE_CUBE:
            return RayCube::intersects(p, d);
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            return RayCylinder::intersects(p, d);
            break;
        case PrimitiveType::PRIMITIVE_TORUS:

            break;
        case PrimitiveType::PRIMITIVE_SPHERE:
            return RaySphere::intersects(p, d);
            break;
        case PrimitiveType::PRIMITIVE_MESH:

            break;
    }

    return 0.0f;
}

RayScene::~RayScene()
{
}

