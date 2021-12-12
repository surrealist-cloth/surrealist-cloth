#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <ishapes/IShape.h>
#include <ishapes/ConeIShape.h>
#include <ishapes/CubeIShape.h>
#include <ishapes/CylinderIShape.h>
#include <ishapes/SphereIShape.h>
#include <ishapes/MeshIShape.h>
#include <ishapes/EmptyIShape.h>
#include <memory>
#include "RGBA.h"
#include <limits>

RayScene::RayScene(Scene &scene) :
    Scene(scene)
{
    std::unique_ptr<MeshIShape> m = std::make_unique<MeshIShape>(); //DEBUG: m_b and m_c are not initalized here, even if they are in the constructor

    // TODO [INTERSECT]
    // Remember that any pointers or OpenGL objects (e.g. texture IDs) will
    // be deleted when the old scene is deleted (assuming you are managing
    // all your memory properly to prevent memory leaks).  As a result, you
    // may need to re-allocate some things here.
    for (auto& primitive : getPrimitives()) {
        switch (primitive.type) {
            case PrimitiveType::PRIMITIVE_CONE:
                m_ishapes.push_back(std::make_unique<ConeIShape>());
                break;
            case PrimitiveType::PRIMITIVE_CUBE:
                m_ishapes.push_back(std::make_unique<CubeIShape>());
                break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                m_ishapes.push_back(std::make_unique<CylinderIShape>());
                break;
            case PrimitiveType::PRIMITIVE_SPHERE:
                m_ishapes.push_back(std::make_unique<SphereIShape>());
                break;
            case PrimitiveType::PRIMITIVE_MESH:
                m_ishapes.push_back(std::make_unique<MeshIShape>());
                break;
            default:
                m_ishapes.push_back(std::make_unique<EmptyIShape>());
        }
    }
}

RayScene::~RayScene()
{
}

glm::vec3 RayScene::getViewplaneCoords(CS123SceneCameraData* camera, int width, int height, float depth, int row, int col)
{
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    float thetaH = glm::radians(camera->heightAngle);
    float thetaW = 2.f * atan(glm::tan(thetaH / 2.f) * aspectRatio);
    float x = (static_cast<float>(col) + 0.5f) / static_cast<float>(width) - 0.5f;
    float y = 0.5f - (static_cast<float>(row) + 0.5f) / static_cast<float>(height);
    return glm::vec3(2 * depth * glm::tan(thetaW / 2.f) * x,
                     2 * depth * glm::tan(thetaH / 2.f) * y,
                     -depth);
}

glm::mat4 RayScene::getCameraTransformation(CS123SceneCameraData *camera)
{
    return glm::lookAt(glm::vec3(camera->pos), glm::vec3(camera->pos + camera->look), glm::vec3(camera->up));
}

glm::vec3 RayScene::illuminate(ShapeIntersection& s, glm::vec3 pos)
{
    CS123SceneGlobalData global = getGlobal();
    glm::vec4 color = global.ka * s.primitive.material.cAmbient;
    for (auto& light : getLights()) {
        glm::vec3 l;

        float attenuation;
        float distance;
        switch (light.type) {
            case LightType::LIGHT_POINT:
                if (!settings.usePointLights) continue;
                l = s.intersection - glm::vec3(light.pos);
                distance = glm::length(l);
                attenuation = light.attenuate(distance);
                l = glm::normalize(l);
                break;
            case LightType::LIGHT_DIRECTIONAL:
                if (!settings.useDirectionalLights) continue;
                l = glm::normalize(glm::vec3(light.dir));
                distance = std::numeric_limits<float>::max();
                attenuation = 1.f; // disable attenuation for directional light
                break;
            default:
                continue;
        }
        glm::vec3 v = glm::normalize(pos - s.intersection);
        glm::vec3 r = glm::reflect(l, s.normal);
        // ray trace to see if the path to the light source is blocked by the object itself
        glm::vec3 eye = s.intersection + s.normal * EPSILON;
        Ray lightRay(eye, -l);

        if (!isOccluded(lightRay, s, distance)) {
            // add texture if enabled
            float blend = 0.f;
            glm::vec4 textureColor = glm::vec4(0);
            if (settings.useTextureMapping) {
                blend = s.primitive.material.blend;
                textureColor = getTexture(s);
            }
            // diffuse + texture
            color += attenuation * light.color * (glm::max(glm::dot(s.normal, -l), 0.f) * (blend * textureColor + global.kd * (1.f - blend) * s.primitive.material.cDiffuse) +
                                                  global.ks * glm::pow(glm::max(glm::dot(v, r), 0.f), s.primitive.material.shininess) * s.primitive.material.cSpecular);
        }
    }
    return glm::vec3(color);
}

glm::vec3 RayScene::rayTrace(Ray& ray)
{
    int maxRecursion = settings.useReflection ? MAX_RECURSION : 1;
    return rayTrace(ray, maxRecursion);
}

glm::vec3 RayScene::rayTrace(Ray &ray, int maxRecursion)
{
    std::unique_ptr<ShapeIntersection> s = intersect(ray);
    // if no intersections, render a single black pixel
    if (!s) {
        return glm::vec3(0, 0, 0);
    }

    glm::vec3 color = illuminate(*s, ray.eye);

    if (maxRecursion > 1) {
        switch (s->primitive.material.type) {
            case MaterialType::MATERIAL_PHONG:
                color = recursePhong(color, ray, *s, maxRecursion - 1);
                break;
            case MaterialType::MATERIAL_METAL:
                break;
            case MaterialType::MATERIAL_GLASS:
                break;
        }
    }


    return color;
}

glm::vec3 RayScene::recursePhong(glm::vec3 color, Ray &ray, ShapeIntersection &s, int maxRecursion)
{
    Ray reflectedRay(s.intersection + s.normal * EPSILON, glm::reflect(ray.dir, s.normal));
    glm::vec3 reflectedColor = glm::vec3(getGlobal().ks * s.primitive.material.cReflective * glm::vec4(rayTrace(reflectedRay, maxRecursion - 1), 1.f));
    return color + reflectedColor;
}

glm::vec3 RayScene::recurseMetal(glm::vec3 color, Ray &ray, ShapeIntersection &s, int maxRecursion)
{
    return color;
}

glm::vec4 RayScene::getTexture(ShapeIntersection &s)
{
    glm::vec3 objectIntersection = glm::vec3(s.invTransformation * glm::vec4(s.intersection, 1));
    std::unique_ptr<glm::vec2> uvPointer = s.ishape.parameterize(objectIntersection);
    glm::vec2 uv = uvPointer ? *uvPointer : glm::vec2(0, 1);
    return s.primitive.material.textureMap.getPixel(uv.x, 1.f - uv.y);
}

bool RayScene::isOccluded(Ray &lightRay, ShapeIntersection &s, float maxT)
{
    if (settings.useShadows) {
        std::unique_ptr<ShapeIntersection> intersection = intersect(lightRay);
        if (!intersection) return false;
        return intersection->t < maxT;
    } else {
        // put light ray in object space
        Ray ray = lightRay;
        ray.transform(s.invTransformation);
        std::unique_ptr<IntersectionCandidate> intersection = s.ishape.closestIntersect(ray);
        if (!intersection) return false;
        return intersection->t < maxT;
    }
}

std::unique_ptr<ShapeIntersection> RayScene::intersect(Ray& ray)
{
    std::vector<CS123ScenePrimitive> primitives = getPrimitives();
    std::vector<glm::mat4> invTransformations = getInverseTransformations();

    assert(invTransformations.size() == primitives.size());

    float matchT = 0.f;
    bool isMatch = false;
    int matchI = 0;
    glm::vec3 matchNormal;
    for (int i = 0; i < m_ishapes.size(); i++) {
        Ray objectRay(ray); // get a copy of the ray
        objectRay.transform(invTransformations[i]);
        std::unique_ptr<IntersectionCandidate> intersection = m_ishapes[i]->closestIntersect(objectRay);
        if (intersection && (!isMatch || intersection->t < matchT)) {
            matchT = intersection->t;
            matchI = i;
            isMatch = true;
            matchNormal = intersection->getNormal(objectRay.getPoint(matchT));
        }
    }

    if (!isMatch) {
        return std::unique_ptr<ShapeIntersection>{};
    }
    glm::vec3 intersection = ray.getPoint(matchT);
    matchNormal = glm::normalize(glm::transpose(glm::mat3(invTransformations[matchI])) * matchNormal);
    // if the normal and the ray form an acute angle, then we should flip the normal, since
    // the ray is coming from the inside of the object
    if (glm::dot(ray.dir, matchNormal) > 0) {
        matchNormal = -matchNormal;
    }

    return std::make_unique<ShapeIntersection>(matchT, intersection, matchNormal, primitives[matchI], *m_ishapes[matchI], invTransformations[matchI]);
}

