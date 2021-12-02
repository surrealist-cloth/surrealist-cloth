#include "ConeShape.h"
#include "PolygonShape.h"
#include "TriangleShape.h"
#include "Triangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <memory>
#include <stdexcept>
#include <vector>

ConeShape::ConeShape(const int nSides, const int nSlices) : m_nSides(nSides), m_nSlices(nSlices)
{
    if (nSides < 3) {
        throw std::runtime_error("nSides must be at least 3");
    }
    if (nSlices < 1) {
        throw std::runtime_error("nSlices must be at least 1");
    }
    ConeShape::generateShape();
}

void ConeShape::generateShape() {
    m_bottom = std::make_unique<PolygonShape>(m_nSides, m_nSlices);
    m_bottom->rotate(glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    m_bottom->rotate(glm::pi<float>() / 2.f, glm::vec3(0.0f, 1.f, 0.0f));
    m_bottom->translate(glm::vec3(0.0f, -0.5f, 0.0f));

    const float angleUnit = glm::pi<float>() * 2.f / static_cast<float>(m_nSides);
    for (int i = 0; i < m_nSides; i++) {
        auto triangle = std::make_unique<TriangleShape>(m_nSlices);
        triangle->translate(glm::vec3(-0.5f, 0.0f, 0.0f));

        const float dividerLength = glm::cos(angleUnit / 2.0f) * 0.5f;
        const float height = glm::sqrt(dividerLength * dividerLength + 1.0f);
        const float sideLength = glm::sin(angleUnit / 2.0f);
        triangle->scale(glm::vec3(height, 1.0f, sideLength));
        triangle->rotate(glm::pi<float>() / 2.0f - glm::atan(dividerLength), glm::vec3(0.0f, 0.0f, 1.0f));
        triangle->rotate((glm::pi<float>() / 2.f) + (angleUnit * (static_cast<float>(i) + 0.5f)), glm::vec3(0.0f, 1.0f, 0.0f));
        triangle->translate(glm::vec3(0.0f, 0.5f, 0.0f));

        triangle->flushTransform();

        m_sides.push_back(std::move(triangle));
    }


    // average the normals between sides
    for (int i = 0; i < m_nSides; i++) {
        int neighborId = (i + 1) % m_nSides;
        std::vector<Triangle>& leftTriangles = m_sides[i]->getTriangles();
        std::vector<Triangle>& rightTriangles = m_sides[neighborId]->getTriangles();
        Normal normal = Triangle::averageNormals({ leftTriangles[0].defaultNormal(), rightTriangles[0].defaultNormal() });
        for (int j = 0; j < (m_nSlices - 1); j++) {
            leftTriangles[2 * j].overrideBNormal(normal);
            leftTriangles[2 * j].overrideCNormal(normal);
            leftTriangles[2 * j + 1].overrideANormal(normal);
            rightTriangles[2 * j].overrideANormal(normal);
            rightTriangles[2 * j + 1].overrideBNormal(normal);
            rightTriangles[2 * j + 1].overrideCNormal(normal);
        }
        normal = Triangle::averageNormals({leftTriangles[leftTriangles.size() - 1].defaultNormal(), rightTriangles[rightTriangles.size() - 1].defaultNormal()});
        leftTriangles[leftTriangles.size() - 1].overrideBNormal(normal);
        rightTriangles[rightTriangles.size() - 1].overrideANormal(normal);
    }
}


std::vector<Shape *> ConeShape::getChildren() {
    std::vector<Shape*> children = {m_bottom.get()};
    for (auto const& shape : m_sides) {
        children.push_back(shape.get());
    }
    return children;
}


