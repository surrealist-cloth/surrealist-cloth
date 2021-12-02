#include "CubeShape.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "RectangleShape.h"
#include <memory>
#include <stdexcept>
#include <vector>

CubeShape::CubeShape(const int n) : m_n(n)
{
    if (n < 1) {
        throw std::runtime_error("n must be at least 1");
    }
    CubeShape::generateShape();
}

void CubeShape::generateShape() {
    m_top = std::make_unique<RectangleShape>(m_n, m_n);
    m_top->translate(glm::vec3(0.0f, 0.5f, 0.0f));
    m_bottom = std::make_unique<RectangleShape>(m_n, m_n);
    m_bottom->rotate(glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    m_bottom->translate(glm::vec3(0.0f, -0.5f, 0.0f));

    m_front = std::make_unique<RectangleShape>(m_n, m_n);
    m_front->rotate(-glm::pi<float>() / 2, glm::vec3(1.0f, 0.0f, 0.0f));
    m_front->translate(glm::vec3(0.0f, 0.0f, -0.5f));
    m_back = std::make_unique<RectangleShape>(m_n, m_n);
    m_back->rotate(glm::pi<float>() / 2, glm::vec3(1.0f, 0.0f, 0.0f));
    m_back->translate(glm::vec3(0.0f, 0.0f, 0.5f));


    m_left = std::make_unique<RectangleShape>(m_n, m_n);
    m_left->rotate(glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f));
    m_left->translate(glm::vec3(-0.5f, 0.0f, 0.0f));
    m_right = std::make_unique<RectangleShape>(m_n, m_n);
    m_right->rotate(-glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, 1.0f));
    m_right->translate(glm::vec3(0.5f, 0.0f, 0.0f));
}

std::vector<Shape*> CubeShape::getChildren() {
    std::vector<Shape*> children = {m_top.get(), m_left.get(), m_front.get(), m_right.get(), m_back.get(), m_bottom.get()};
    return children;
}
