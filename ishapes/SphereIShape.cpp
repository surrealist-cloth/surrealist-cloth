#include "SphereIShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <cmath>
#include <math.h>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>

SphereIShape::SphereIShape()
{

}

std::vector<IntersectionCandidate> SphereIShape::intersect(Ray& ray) const
{
    float a = pow(ray.dir.x, 2.f) + pow(ray.dir.y, 2.f) + pow(ray.dir.z, 2.f);
    float b = 2.f * ray.eye.x * ray.dir.x + 2.f * ray.eye.y * ray.dir.y + 2.f * ray.eye.z * ray.dir.z;
    float c = pow(ray.eye.x, 2.f) + pow(ray.eye.y, 2.f) + pow(ray.eye.z, 2.f) - 0.25;
    std::vector<IntersectionCandidate> ts;
    for (float t: solveQuadratic(a, b, c)) {
        ts.push_back(IntersectionCandidate(t, [&](glm::vec3 point) {
            return glm::vec3(2 * point.x, 2 * point.y, 2 * point.z);
        }));
    }
     return ts;
}


std::unique_ptr<glm::vec2> SphereIShape::parameterize(glm::vec3 &point) const
{
    // do top tip
    if (glm::epsilonEqual(point.y, 0.5f, EPSILON)) {
        return std::make_unique<glm::vec2>(0.5f, 1.f);
    }
    // do bottom tip
    if (glm::epsilonEqual(point.y, -0.5f, EPSILON)) {
        return std::make_unique<glm::vec2>(0.5f, 0.f);
    }
    if (point.y > 0.5 || point.y < -0.5) {
        return std::unique_ptr<glm::vec2>(nullptr);
    }
    // do body
    float theta = atan2(point.z, point.x);
    float u = (theta < 0) ? (-theta / (2 * glm::pi<float>())) : (1 - theta / (2 * glm::pi<float>()));
    float phi = asin(point.y / 0.5f);
    float v = glm::min(glm::max(phi / glm::pi<float>() + 0.5f, 0.f), 1.f);

    return std::make_unique<glm::vec2>(u, v);
}
