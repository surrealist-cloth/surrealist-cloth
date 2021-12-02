#include "Triangle.h"
#include "glm/glm.hpp"

#include <initializer_list>

Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) : m_a(a), m_b(b), m_c(c) {
    computeDefaultNormal();
}

void Triangle::computeDefaultNormal() {
    m_defaultNormal = glm::normalize(glm::cross(m_b - m_a, m_c - m_a));
}

inline glm::vec3 apply_transformation(glm::vec3 v, glm::mat4 transformation) {
    return glm::vec3(transformation * glm::vec4(v, 1.0f));
}

void Triangle::transform(glm::mat4 transformation)
{
    m_a = apply_transformation(m_a, transformation);
    m_b = apply_transformation(m_b, transformation);
    m_c = apply_transformation(m_c, transformation);
    computeDefaultNormal();
}

Normal Triangle::averageNormals(const std::initializer_list<Normal> &normals) {
    Normal aggregate;
    for (const Normal &normal : normals) {
        aggregate += normal;
    }
    return glm::normalize(aggregate);
}
