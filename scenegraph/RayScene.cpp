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
#include <ishapes/EmptyIShape.h>
#include <memory>
#include "RGBA.h"
#include <limits>
#include <glm/gtx/extented_min_max.hpp>

RayScene::RayScene(Scene &scene) :
    Scene(scene)
{
    // TODO [INTERSECT]
    // Remember that any pointers or OpenGL objects (e.g. texture IDs) will
    // be deleted when the old scene is deleted (assuming you are managing
    // all your memory properly to prevent memory leaks).  As a result, you
    // may need to re-allocate some things here.
    for (auto& primitive : getPrimitives()) {
        switch (primitive.type) {
            case PrimitiveType::PRIMITIVE_CONE:
                m_ishapes.push_back(ConeIShape::shared_instance());
                break;
            case PrimitiveType::PRIMITIVE_CUBE:
                m_ishapes.push_back(CubeIShape::shared_instance());
                break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                m_ishapes.push_back(CylinderIShape::shared_instance());
                break;
            case PrimitiveType::PRIMITIVE_SPHERE:
                m_ishapes.push_back(SphereIShape::shared_instance());
                break;
            default:
                m_ishapes.push_back(EmptyIShape::shared_instance());
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
        Ray lightRay(s.intersection, -l, EPSILON);

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

glm::vec3 RayScene::rayTrace(Ray &ray)
{
    if (!settings.useReflection || ray.depth == MAX_RECURSION || ray.radiance.a * glm::max(ray.radiance.r, ray.radiance.g, ray.radiance.b) < MIN_INTENSITY) {
        return glm::vec3(0);
    }

    std::unique_ptr<ShapeIntersection> s = intersect(ray);
    // if no intersections, render a single black pixel
    if (!s) {
        return glm::vec3(0);
    }

    switch (s->primitive.material.type) {
        case MaterialType::MATERIAL_PHONG:
            return renderPhong(ray, *s);
        case MaterialType::MATERIAL_METAL:
            return renderMetal(ray, *s);
        case MaterialType::MATERIAL_GLASS:
            return renderGlass(ray, *s);
        default:
            return glm::vec3(0);
    }
}

glm::vec3 RayScene::renderPhong(Ray &ray, ShapeIntersection &s)
{
    glm::vec3 color = illuminate(s, ray.eye);
    glm::vec4 multiplier = getGlobal().ks * s.primitive.material.cReflective;
    Ray reflectedRay(s.intersection, glm::reflect(ray.dir, s.normal), EPSILON, ray.depth + 1, ray.radiance * multiplier);
    glm::vec3 reflectedColor = glm::vec3(multiplier * glm::vec4(rayTrace(reflectedRay), 1.f));
    return color + reflectedColor;
}

glm::vec3 RayScene::renderMetal(Ray &ray, ShapeIntersection &s)
{
    glm::vec3 color = illuminate(s, ray.eye);
    return color;
}

glm::vec3 RayScene::renderGlass(Ray &ray, ShapeIntersection &s)
{
    glm::vec3 ior(s.primitive.material.ior.r, s.primitive.material.ior.g, s.primitive.material.ior.b);
    glm::vec3 eta = s.isInside ? ior : 1.f / ior;
    float fresnelR = fresnel(ior.r, s.normal, ray.dir);
    float fresnelG = fresnel(ior.g, s.normal, ray.dir);
    float fresnelB = fresnel(ior.b, s.normal, ray.dir);
    glm::vec3 fresnelCoefficient(fresnelR, fresnelG, fresnelB);

    Ray reflect(s.intersection, glm::reflect(ray.dir, s.normal), EPSILON, ray.depth + 1, ray.radiance * glm::vec4(fresnelCoefficient, 1.f));
    glm::vec3 reflectionColor = rayTrace(reflect);

    Ray refractR(s.intersection, glm::refract(ray.dir, s.normal, eta.r), EPSILON, ray.depth + 1, ray.radiance * glm::vec4(fresnelR, 0.f, 0.f, 1.f));
    Ray refractG(s.intersection, glm::refract(ray.dir, s.normal, eta.g), EPSILON, ray.depth + 1, ray.radiance * glm::vec4(0.f, fresnelG, 0.f, 1.f));
    Ray refractB(s.intersection, glm::refract(ray.dir, s.normal, eta.b), EPSILON, ray.depth + 1, ray.radiance * glm::vec4(0.f, 0.f, fresnelB, 1.f));
    float refractionColorR = rayTrace(refractR).r;
    float refractionColorG = rayTrace(refractG).g;
    float refractionColorB = rayTrace(refractB).b;
    glm::vec3 refractionColor(refractionColorR, refractionColorG, refractionColorB);

    return (1.f - fresnelCoefficient) * refractionColor + fresnelCoefficient * reflectionColor;
}

float RayScene::fresnel(float ior, glm::vec3 normal, glm::vec3 raydir)
{
    float r0 = glm::pow((ior - 1.f) / (ior + 1.f), 2.f);
    return glm::mix(glm::pow(1.f - glm::dot(normal, -raydir), 5.f), 1.f, r0);
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
        std::unique_ptr<float> intersection = s.ishape.closestIntersect(ray);
        if (!intersection) return false;
        return *intersection < maxT;
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
    glm::vec3 matchPoint;
    for (int i = 0; i < m_ishapes.size(); i++) {
        Ray objectRay(ray); // get a copy of the ray
        objectRay.transform(invTransformations[i]);
        std::unique_ptr<float> intersection = m_ishapes[i].get().closestIntersect(objectRay);
        if (intersection && (!isMatch || *intersection < matchT)) {
            matchT = *intersection;
            matchI = i;
            matchPoint = objectRay.getPoint(matchT); // point in object space
            isMatch = true;
        }
    }

    if (!isMatch) {
        return std::unique_ptr<ShapeIntersection>{};
    }
    std::unique_ptr<glm::vec3> normalPointer = m_ishapes[matchI].get().getNormal(matchPoint);
    glm::vec3 intersection = ray.getPoint(matchT);
    glm::vec3 normal = normalPointer ? *normalPointer : glm::vec3(0, 1, 0);
    normal = glm::normalize(glm::transpose(glm::mat3(invTransformations[matchI])) * normal);
    // if the normal and the ray form an acute angle, then we should flip the normal, since
    // the ray is coming from the inside of the object
    bool isInside = glm::dot(ray.dir, normal) > 0;
    if (isInside) normal = -normal;

    return std::make_unique<ShapeIntersection>(matchT, intersection, normal, primitives[matchI], m_ishapes[matchI], invTransformations[matchI], isInside);
}

