#include "CylinderShape.h"
#include "PolygonShape.h"
#include "RectangleShape.h"
#include "Triangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <memory>
#include <stdexcept>
#include <vector>

CylinderShape::CylinderShape(const int nSides, const int nSlices) : m_nSides(nSides), m_nSlices(nSlices)
{
    if (nSides < 3) {
        throw std::runtime_error("nSides must be at least 3");
    }
    if (nSlices < 1) {
        throw std::runtime_error("nSlices must be at least 1");
    }
    CylinderShape::generateShape();
}

void CylinderShape::generateShape() {
    const float angleUnit = glm::pi<float>() * 2.f / static_cast<float>(m_nSides);
    m_top = std::make_unique<PolygonShape>(m_nSides, m_nSlices);
    m_top->translate(glm::vec3(0.0f, 0.5f, 0.0f));
    m_top->rotate(glm::pi<float>() / 2.f, glm::vec3(0.0f, 1.0f, 0.0f));

    m_bottom = std::make_unique<PolygonShape>(m_nSides, m_nSlices);
    m_bottom->rotate(glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    m_bottom->translate(glm::vec3(0.0f, -0.5f, 0.0f));
    m_bottom->rotate(glm::pi<float>() / 2.f, glm::vec3(0.0f, 1.0f, 0.0f));

    for (int i = 0; i < m_nSides; i++) {
        auto rectangle = std::make_unique<RectangleShape>(m_nSlices, 1);
        rectangle->rotate(glm::pi<float>() / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        rectangle->scale(glm::vec3(1.0f, 1.0f, glm::sin(angleUnit / 2.0f)));
        rectangle->rotate(glm::pi<float>() / 2.f, glm::vec3(0.0f, 1.f, 0.f));
        rectangle->translate(glm::vec3(0.f, 0.f, glm::cos(angleUnit * 0.5f) * 0.5f));

        rectangle->rotate(angleUnit * (static_cast<float>(i) + 0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        rectangle->flushTransform();

        m_sides.push_back(std::move(rectangle));
    }

    // average the normals between sides
    for (int i = 0; i < m_nSides; i++) {
        int neighborId = (i + 1) % m_nSides;
        std::vector<Triangle>& leftTriangles = m_sides[i].get()->getTriangles();
        std::vector<Triangle>& rightTriangles = m_sides[neighborId].get()->getTriangles();
        Normal normal = Triangle::averageNormals({ leftTriangles[0].defaultNormal(), rightTriangles[0].defaultNormal() });
        for (int j = 0; j < m_nSlices; j++) {
            leftTriangles[2 * j].overrideBNormal(normal);
            leftTriangles[2 * j + 1].overrideANormal(normal);
            leftTriangles[2 * j + 1].overrideBNormal(normal);
            rightTriangles[2 * j].overrideANormal(normal);
            rightTriangles[2 * j].overrideCNormal(normal);
            rightTriangles[2 * j + 1].overrideCNormal(normal);
        }
    }
}


std::vector<Shape *> CylinderShape::getChildren() {
    std::vector<Shape*> children = {m_top.get(), m_bottom.get()};
    for (auto const& shape : m_sides) {
        children.push_back(shape.get());
    }
    return children;
}


