#include "ConeIShape.h"
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include "scenegraph/RayScene.h"
#include "ishapes/CircleIShape.h"

ConeIShape::ConeIShape()
{
    m_bottom = std::make_unique<CircleIShape>(-0.5, -1);
}

std::vector<float> ConeIShape::intersect(Ray &ray) const
{
    // intersect bottom circle
    std::vector<float> ts = m_bottom->allIntersect(ray);
    float a = pow(ray.dir.x, 2.f) + pow(ray.dir.z, 2.f) - pow(ray.dir.y, 2.f) / 4.f;
    float b = 2 * ray.eye.x * ray.dir.x + 2 * ray.eye.z * ray.dir.z - ray.eye.y * ray.dir.y / 2.f + ray.dir.y / 4;
    float c = pow(ray.eye.x, 2.f) + pow(ray.eye.z, 2.f) - pow(ray.eye.y, 2.f) / 4.f + ray.eye.y / 4.f - 1 / 16.f;
    std::vector<float> otherTs = solveQuadratic(a, b, c);
    std::copy_if(otherTs.begin(), otherTs.end(), std::back_inserter(ts), [ray](float t) {
        float y = ray.getPoint(t).y;
        return (y <= 0.5) && (y >= -0.5);
    });
    return ts;
}

std::unique_ptr<glm::vec3> ConeIShape::getNormal(glm::vec3& point) const
{
    // do bottom circle
    if (glm::epsilonEqual(point.y, -0.5f, EPSILON)) {
        return m_bottom->getNormal(point);
    }
    // handle top tip
    if (glm::epsilonEqual(point.y, 0.5f, EPSILON)) {
        return std::make_unique<glm::vec3>(0.f, 1.f, 0.f);
    }
    return std::make_unique<glm::vec3>(2 * point.x, - point.y / 2.f + 1 / 4.f, 2 * point.z);
}

std::unique_ptr<glm::vec2> ConeIShape::parameterize(glm::vec3 &point) const
{
    // do top tip
    if (glm::epsilonEqual(point.y, 0.5f, EPSILON)) {
        return std::make_unique<glm::vec2>(0.5f, 1.f);
    }
    // do bottom circle
    if (glm::epsilonEqual(point.y, -0.5f, EPSILON)) {
        return m_bottom->parameterize(point);
    }
    if (point.y > 0.5 || point.y < -0.5) {
        return std::unique_ptr<glm::vec2>(nullptr);
    }
    // do body
    float theta = atan2(point.z, point.x);
    float u = (theta < 0) ? (-theta / (2 * glm::pi<float>())) : (1 - theta / (2 * glm::pi<float>()));
    float v = glm::min(glm::max(point.y + 0.5f, 0.f), 1.f);
    return std::make_unique<glm::vec2>(u, v);
}
