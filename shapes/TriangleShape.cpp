#include "TriangleShape.h"
#include <stdexcept>

TriangleShape::TriangleShape(const int n) : m_n(n)
{
    if (n < 1) {
        throw std::runtime_error("n must be at least 1");
    }
    TriangleShape::generateShape();
}

void TriangleShape::generateShape() {
    const float START = -0.5f;
    const float END = 0.5f;
    const float LENGTH = END - START;

    const float unitX = LENGTH / static_cast<float>(m_n);
    const float unitY = unitX / 2.f;
    for (int i = 0; i < m_n; i++) {
        glm::vec3 a(START + i * unitX, 0.0f, START + i * unitY);
        glm::vec3 b(START + i * unitX, 0.0f, END - i * unitY);
        glm::vec3 c(START + (i+1) * unitX, 0.0f, END - (i+1) * unitY);
        glm::vec3 d(START + (i+1) * unitX, 0.0f, START + (i+1) * unitY);
        m_triangles.push_back(Triangle(a, b, c));
        if (i != (m_n - 1)) {
           m_triangles.push_back(Triangle(c, d, a));
        }
    }
}
