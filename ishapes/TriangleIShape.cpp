#include "TriangleIShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include "scenegraph/RayScene.h"
#include "ishapes/IShape.h"

/**
 * @brief TriangleIShape::TriangleIShape
 * Points must be specified in counter clockwise order!
 * @param a 1st point of triangle
 * @param b 2nd point
 * @param c 3rd point
 */
TriangleIShape::TriangleIShape(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c)
{
    float a_x = a.x;
    float a_y = a.y;
    float a_z = a.z;

    float b_x = b.x;
    float b_y = b.y;
    float b_z = b.z;

    float c_x = c.x;
    float c_y = c.y;
    float c_z = c.z;

    m_a = std::make_unique<glm::vec3>(a_x, a_y, a_z);
    m_b = std::make_unique<glm::vec3>(b_x, b_y, b_z);
    m_c = std::make_unique<glm::vec3>(c_x, c_y, c_z);

    m_n = std::make_unique<glm::vec3>(
                glm::normalize(glm::cross(*m_b - *m_a, *m_c - *m_a))
                );

//    m_a = std::make_unique<glm::vec3>(0.0f, -0.5f, 0.0f);
//    m_b = std::make_unique<glm::vec3>(0.5f, -0.5f, 0.0f);
//    m_c = std::make_unique<glm::vec3>(0.0f, 0.5f, 0.0f);
}


std::vector<IntersectionCandidate> TriangleIShape::intersect(Ray &ray) const {


    std::vector<IntersectionCandidate> ts;
    float t;

    if (m_a == m_b || m_b == m_c || m_c == m_a) {
        return ts;
    }

    float d = glm::dot(*m_n, *m_a);

    // find point p + dt on the triangle's plane
    if (glm::epsilonEqual(glm::dot(*m_n, ray.dir), 0.0f, EPSILON)) {
        return ts;
    } // d is parallel to plane
    t = (d - glm::dot(*m_n, ray.eye)) / (glm::dot(*m_n, ray.dir));

    // check if ray intersection with plane is inside triangle
    // (see if on the inside line of all edges by checking normal's direction)
    if (glm::dot(glm::cross(*m_b - *m_a, (ray.eye + t * ray.dir) - *m_a), *m_n) < 0) {
        return ts;
    }
    if (glm::dot(glm::cross(*m_c - *m_b, (ray.eye + t * ray.dir) - *m_b), *m_n) < 0) {
        return ts;
    }
    if (glm::dot(glm::cross(*m_a - *m_c, (ray.eye + t * ray.dir) - *m_c), *m_n) < 0) {
        return ts;
    }

    ts.push_back(IntersectionCandidate(t, [&](glm::vec3 point){
                                                            return *m_n;
                                      })
                );
    return ts;
}

std::unique_ptr<glm::vec2> TriangleIShape::parameterize(glm::vec3 &point) const
{
    return std::make_unique<glm::vec2>(0,0);
}

bool TriangleIShape::isInTriangle(glm::vec3 &point) const {
    if (! glm::epsilonEqual(glm::dot(*m_n, point), glm::dot(*m_n, *m_a), EPSILON) ) {
        return false;
    }
    if (! isWithinTriangle(point)) {
        return false;
    }
    return true;
}

bool TriangleIShape::isWithinTriangle(glm::vec3 &point) const {
    if (glm::dot(glm::cross(*m_b - *m_a, (point) - *m_a), *m_n) < 0) {
        return false;
    }
    if (glm::dot(glm::cross(*m_c - *m_b, (point) - *m_b), *m_n) < 0) {
        return false;
    }
    if (glm::dot(glm::cross(*m_a - *m_c, (point) - *m_c), *m_n) < 0) {
        return false;
    }
    return true;
}

std::unique_ptr<glm::vec3> TriangleIShape::getSurfaceNormal() const {
    return std::make_unique<glm::vec3>(m_n->x, m_n->y, m_n->z);
}
