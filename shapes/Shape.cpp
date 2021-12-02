#include "Shape.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

std::vector<Triangle> Shape::getTrianglesRecursive() {
    flushTransform();

    std::vector<Triangle> triangles = m_triangles;
    m_transformation = glm::mat4(1.0f);
    for (Shape* child : getChildren()) {
        std::vector<Triangle> childTriangles = child->getTrianglesRecursive();
        triangles.insert(triangles.end(), childTriangles.begin(), childTriangles.end());
    }
    return triangles;
}

std::vector<Triangle>& Shape::getTriangles() {
    return m_triangles;
}

void Shape::translate(glm::vec3 delta) {
    transform(glm::translate(glm::mat4(1.0f), delta));
}

void Shape::rotate(float angle, glm::vec3 axis) {
    transform(glm::rotate(glm::mat4(1.0f), angle, axis));
}

void Shape::scale(glm::vec3 scales) {
    transform(glm::scale(glm::mat4(1.0f), scales));
}

void Shape::transform(glm::mat4 transformation) {
    m_transformation = transformation * m_transformation;
    for (Shape* child : getChildren()) {
        child->transform(transformation);
    }
}

void Shape::flushTransform() {
    if (m_transformation == glm::mat4(1.0f)) return;
    // apply the built-up transformation and reset the matrix
    for (int i = 0; i < m_triangles.size(); i++) {
        m_triangles[i].transform(m_transformation);
    }
    m_transformation = glm::mat4(1.0f);
}
