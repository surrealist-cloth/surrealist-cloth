#include "CylinderIShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include "scenegraph/RayScene.h"
#include "ishapes/CircleIShape.h"
#include <cmath>
#include <algorithm>
#include <math.h>

CylinderIShape::CylinderIShape()
{
    m_top = std::make_unique<CircleIShape>(0.5, 1);
    m_bottom = std::make_unique<CircleIShape>(-0.5, -1);
}

std::vector<IntersectionCandidate> CylinderIShape::intersect(Ray &ray) const
{
    // intersect top circle
    std::vector<IntersectionCandidate> ts = m_top->allIntersect(ray);
    // intersect bottom circle
    std::vector<IntersectionCandidate> bottomTs = m_bottom->allIntersect(ray);
    ts.insert(ts.end(), bottomTs.begin(), bottomTs.end());

    float a = pow(ray.dir.x, 2.f) + pow(ray.dir.z, 2.f);
    float b = 2 * ray.eye.x * ray.dir.x + 2 * ray.eye.z * ray.dir.z;
    float c = pow(ray.eye.x, 2.f) + pow(ray.eye.z, 2.f) - 1 / 4.f;
    std::vector<float> bodyTs = solveQuadratic(a, b, c);


    for (float t: bodyTs) {
        float y = ray.getPoint(t).y;
        if ((y <= 0.5) && (y >= -0.5)) {
                    ts.push_back(IntersectionCandidate(t, [&](glm::vec3 point) {
                                 return glm::vec3(2 * point.x, 0, 2 * point.z);
                                 }
                                 ));
        }

    }
    return ts;
}


std::unique_ptr<glm::vec2> CylinderIShape::parameterize(glm::vec3 &point) const
{
    // do top circle
    if (glm::epsilonEqual(point.y, 0.5f, EPSILON)) {
        std::unique_ptr<glm::vec2> uvPointer = m_top->parameterize(point);
        glm::vec2 uv = uvPointer ? *uvPointer : glm::vec2(1, 0);
        return std::make_unique<glm::vec2>(uv.x, 1.f - uv.y);
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
    float v =  glm::min(glm::max(point.y + 0.5f, 0.f), 1.f);
    return std::make_unique<glm::vec2>(u, v);
}
