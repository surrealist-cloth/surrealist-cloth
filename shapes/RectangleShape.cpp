#include "RectangleShape.h"
#include <stdexcept>

RectangleShape::RectangleShape(const int m, const int n) : m_m(m), m_n(n)
{
    if (m < 1) {
        throw std::runtime_error("m must be at least 1");
    }
    if (n < 1) {
        throw std::runtime_error("n must be at least 1");
    }
    RectangleShape::generateShape();
}

void RectangleShape::generateShape() {
    const float START = -0.5f;
    const float LENGTH = 1.0f;
    const float unitX = LENGTH / static_cast<float>(m_m);
    const float unitY = LENGTH / static_cast<float>(m_n);
    for (int i = 0; i < m_m; i++) {
        for (int j = 0; j < m_n; j++) {
            glm::vec3 a(START + unitX * i, 0, START + unitY * j);
            glm::vec3 b = a + glm::vec3(unitX, 0, 0);
            glm::vec3 c = b + glm::vec3(0, 0, unitY);
            glm::vec3 d = a + glm::vec3(0, 0, unitY);
            m_triangles.push_back(Triangle(a, d, b));
            m_triangles.push_back(Triangle(d, c, b));
        }
    }
}
