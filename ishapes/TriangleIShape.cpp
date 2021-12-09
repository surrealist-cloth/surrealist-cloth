#include "TriangleIShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include "scenegraph/RayScene.h"

/**
 * @brief TriangleIShape::TriangleIShape
 * Points must be specified in counter clockwise order!
 * @param a 1st point of triangle
 * @param b 2nd point
 * @param c 3rd point
 */
TriangleIShape::TriangleIShape(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c)
{
    m_a = std::make_unique<glm::vec3>(-0.5f, -0.5f, 0.0f);
    m_b = std::make_unique<glm::vec3>(-0.5f, -0.5f, 0.0f);
    m_c = std::make_unique<glm::vec3>(-0.5f, -0.5f, 0.0f);
}


std::vector<float> TriangleIShape::intersect(Ray &ray) const {

    std::vector<float> ts;
    float t;

    glm::vec3 n = glm::normalize(glm::cross(*m_b - *m_a, *m_c - *m_a));
    float d = glm::dot(n, *m_a);

    // find point p + dt on the triangle's plane
    if (glm::dot(n, ray.dir)) return ts; // d is parallel to plane
    t = (d - glm::dot(n, ray.eye)) / (glm::dot(n, ray.dir));

    // check if ray intersection with plane is inside triangle
    // (see if on the inside line of all edges by checking normal's direction)
    if (glm::dot(glm::cross(*m_b - *m_a, (ray.eye + t * ray.dir) - *m_a), n) < 0) return ts;
    if (glm::dot(glm::cross(*m_c - *m_b, (ray.eye + t * ray.dir) - *m_b), n) < 0) return ts;
    if (glm::dot(glm::cross(*m_a - *m_c, (ray.eye + t * ray.dir) - *m_c), n) < 0) return ts;

    ts.push_back(t);
    return ts;
}

std::unique_ptr<glm::vec3> TriangleIShape::getNormal(glm::vec3& point) const {
    return std::make_unique<glm::vec3>(glm::normalize(glm::cross(*m_b - *m_a, *m_c - *m_a)));
}

std::unique_ptr<glm::vec2> TriangleIShape::parameterize(glm::vec3 &point) const
{
    return std::make_unique<glm::vec2>(0,0);
}
