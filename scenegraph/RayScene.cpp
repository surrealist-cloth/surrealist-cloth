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
#include <glm/gtx/extented_min_max.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/random.hpp>

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
                m_ishapes.push_back(std::make_unique<MeshIShape>(primitive.meshfile));
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

glm::vec3 RayScene::sampleGGX(float roughness, glm::vec3 normal, glm::vec3 raydir)
{
    float e1 = glm::linearRand(0.f, 1.f);
    float e2 = glm::linearRand(0.f, 1.f);
    float theta = glm::acos(glm::sqrt((1.f - e1) / ((glm::pow(roughness, 2.f) - 1.f) * e1 + 1.f)));
    float phi = 2.f * glm::pi<float>() * e2;
    glm::vec3 v(glm::cos(phi) * glm::sin(theta),
                glm::sin(phi) * glm::sin(theta),
                glm::cos(theta));
    return tangentSpaceToWorldSpace(v, normal, raydir);
}

glm::vec3 RayScene::tangentSpaceToWorldSpace(glm::vec3 v, glm::vec3 normal, glm::vec3 raydir)
{
    // use an arbitrary orthogonal vector as tangent if raydir is parallel to normal
    if (glm::all(glm::epsilonEqual(raydir, normal, EPSILON))) {
        raydir = glm::vec3(normal.x + 1, normal.y, normal.z);
    }
    glm::vec3 tangent = glm::normalize(raydir - glm::proj(raydir, normal));
    glm::vec3 binormal = glm::cross(tangent, normal);
    glm::mat3 tbn(tangent, binormal, normal);
    return glm::normalize(tbn * v);
}

glm::vec3 RayScene::getIOR(ShapeIntersection &s)
{
    return settings.useRGBIOR ?
                    glm::vec3(s.primitive.material.ior.r, s.primitive.material.ior.g, s.primitive.material.ior.b) :
                    glm::vec3((s.primitive.material.ior.r + s.primitive.material.ior.g + s.primitive.material.ior.b) / 3.f);
}

glm::vec3 RayScene::rayTrace(Ray& ray)
{
    if ((!settings.useRecursive && ray.depth > 0) || ray.depth == MAX_RECURSION || ray.radiance.a * glm::max(ray.radiance.r, ray.radiance.g, ray.radiance.b) < MIN_INTENSITY) {
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

glm::vec3 RayScene::renderPhong(Ray& ray, ShapeIntersection &s)
{
    if (s.isInside) {
        return glm::vec3(0);
    }
    glm::vec3 color = illuminate(s, ray.eye);
    glm::vec4 multiplier = getGlobal().ks * s.primitive.material.cReflective;
    Ray reflectedRay(s.intersection, glm::reflect(ray.dir, s.normal), EPSILON, ray.depth + 1, ray.radiance * multiplier);
    glm::vec3 reflectedColor = glm::vec3(multiplier * glm::vec4(rayTrace(reflectedRay), 1.f));
    return glm::clamp(color + reflectedColor, 0.f, 1.f);
}

glm::vec3 RayScene::renderMetal(Ray& ray, ShapeIntersection &s)
{
    glm::vec3 objectColor = illuminate(s, ray.eye);

    glm::vec3 specularColor(0);
    glm::vec3 cSpecular(0);

    const int SAMPLE_COUNT = 32;
    #pragma omp parallel for
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        glm::vec3 sampleNormal = sampleGGX(s.primitive.material.roughness, s.normal, ray.dir);
        glm::vec3 sampleDir = glm::normalize(glm::reflect(ray.dir, sampleNormal));
        if ((glm::dot(sampleDir, s.normal) < 0.f) || (glm::dot(sampleDir, sampleNormal) < 0.f)) continue;
        glm::vec3 fresnel = getFresnel(glm::vec3(s.primitive.material.cReflective), sampleNormal, sampleDir);
        float geometricAttenuation = getGeometricAttenuation(sampleDir, ray.dir, s.normal, s.primitive.material.roughness);

        glm::vec3 multiplier = glm::clamp(fresnel *
                                          geometricAttenuation *
                                          glm::abs(glm::dot(ray.dir, sampleNormal) / (glm::dot(ray.dir, s.normal) * glm::dot(sampleNormal, s.normal))),
                                          0.f, 1.f);

        Ray reflectedRay(s.intersection, sampleDir, EPSILON, ray.depth + 1, ray.radiance * glm::vec4(multiplier, 1.f));
        glm::vec3 reflectedColor = multiplier * rayTrace(reflectedRay);

        #pragma omp atomic
        cSpecular += fresnel;

        #pragma omp atomic
        specularColor += reflectedColor;
    }
    cSpecular = cSpecular / static_cast<float>(SAMPLE_COUNT);
    specularColor = specularColor / static_cast<float>(SAMPLE_COUNT);
    return glm::clamp(specularColor + objectColor * (1.f - cSpecular), 0.f, 1.f);
}

glm::vec3 RayScene::renderGlass(Ray& ray, ShapeIntersection &s)
{
    glm::vec3 ior = getIOR(s);
    glm::vec3 fresnel = getFresnelDielectric(ior, s.normal, ray.dir);
    Ray reflect(s.intersection, glm::reflect(ray.dir, s.normal), EPSILON, ray.depth + 1, ray.radiance * glm::vec4(fresnel, 1.f));
    glm::vec3 reflectionColor = rayTrace(reflect);

    glm::vec3 eta = s.isInside ? ior : 1.f / ior;
    glm::vec3 refractionColor;
    if (settings.useRGBIOR) {
        Ray refractR(s.intersection, glm::refract(ray.dir, s.normal, eta.r), EPSILON, ray.depth + 1, ray.radiance * glm::vec4(fresnel.r, 0.f, 0.f, 1.f));
        Ray refractG(s.intersection, glm::refract(ray.dir, s.normal, eta.g), EPSILON, ray.depth + 1, ray.radiance * glm::vec4(0.f, fresnel.g, 0.f, 1.f));
        Ray refractB(s.intersection, glm::refract(ray.dir, s.normal, eta.b), EPSILON, ray.depth + 1, ray.radiance * glm::vec4(0.f, 0.f, fresnel.b, 1.f));
        refractionColor = glm::vec3(rayTrace(refractR).r,
                                    rayTrace(refractG).g,
                                    rayTrace(refractB).b);
    } else {
        Ray refract(s.intersection, glm::refract(ray.dir, s.normal, eta.r), EPSILON, ray.depth + 1, ray.radiance * glm::vec4(fresnel, 1.f));
        refractionColor = rayTrace(refract);
    }
    return glm::clamp((1.f - fresnel) * refractionColor + fresnel * reflectionColor, 0.f, 1.f);
}

glm::vec3 RayScene::getFresnelDielectric(glm::vec3 ior, glm::vec3 normal, glm::vec3 raydir)
{
    glm::vec3 r0 = (ior - 1.f) / (ior + 1.f);
    r0 = r0 * r0;
    return getFresnel(r0, normal, raydir);
}

glm::vec3 RayScene::getFresnel(glm::vec3 r0, glm::vec3 normal, glm::vec3 raydir) {
    if (glm::dot(raydir, normal) < 0.f) raydir = -raydir;
    return glm::clamp(r0 + (1.f - r0) * glm::pow(1.f - glm::dot(normal, raydir), 5.f), 0.f, 1.f);
}


float RayScene::chi(float v)
{
    return v > 0 ? 1.f : 0.f;
}

float RayScene::partialGeometricTerm(glm::vec3 v, glm::vec3 normal, glm::vec3 half, float m)
{
    float product = glm::clamp(glm::dot(v, half), 0.f, 1.f);
    float chiMultiplier = chi(product / glm::clamp(glm::dot(v, normal), 0.f, 1.f));
    product = product * product;
    float tan2 = (1.f - product) / product;
    return chiMultiplier * 2.f / (1.f + glm::sqrt(1 + m * m * tan2));
}

float RayScene::getGeometricAttenuation(glm::vec3 sampleDir, glm::vec3 rayDir, glm::vec3 normal, float m)
{
    float m2 = m * m;
    float GI = glm::dot(sampleDir, normal);
    float GO = glm::dot(-rayDir, normal);

    return 2.f * GI * GO / (GO * glm::sqrt(m2 + (1 - m2) * glm::pow(GI, 2.f)) + GI * glm::sqrt(m2 + (1 - m2) * glm::pow(GO, 2.f)));
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
    if (fabs(matchNormal.z) > 0.9) {
        int i =0;
    }
    // if the normal and the ray form an acute angle, then we should flip the normal, since
    // the ray is coming from the inside of the object
    bool isInside = glm::dot(ray.dir, normal) > 0;
    if (isInside) normal = -normal;

    return std::make_unique<ShapeIntersection>(matchT, intersection, normal, primitives[matchI], m_ishapes[matchI], invTransformations[matchI], isInside);
}

