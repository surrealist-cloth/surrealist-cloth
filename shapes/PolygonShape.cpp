#include "PolygonShape.h"
#include "TriangleShape.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include <memory>
#include <stdexcept>
#include <vector>

PolygonShape::PolygonShape(const int nSides, const int nSlices) : m_nSides(nSides), m_nSlices(nSlices)
{
    if (nSides < 3) {
        throw std::runtime_error("nSides must be at least 3");
    }
    if (nSlices < 1) {
        throw std::runtime_error("nSlices must be at least 1");
    }
    PolygonShape::generateShape();
}


void PolygonShape::generateShape() {
    const float angleUnit = glm::pi<float>() * 2.f / static_cast<float>(m_nSides);
    for (int i = 0; i < m_nSides; i++) {
        auto triangle = std::make_unique<TriangleShape>(m_nSlices);
        triangle->translate(glm::vec3(-0.5f, 0.0f, 0.0f));
        triangle->scale(glm::vec3(glm::cos(angleUnit / 2.0f) / 2, 1.0f, glm::sin(angleUnit / 2.0f)));
        triangle->rotate(angleUnit * (i + 0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        m_slices.push_back(std::move(triangle));
    }
}

std::vector<Shape*> PolygonShape::getChildren() {
    std::vector<Shape*> children;
    for (auto const& shape : m_slices) {
        children.push_back(shape.get());
    }
    return children;
}
