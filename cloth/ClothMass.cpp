#include "ClothMass.h"

ClothMass::~ClothMass()
{
}

ClothMass::ClothMass() : m_mass(1.f), m_position(std::make_unique<glm::vec3>())
{
}

ClothMass::ClothMass(float mass) : m_mass(mass), m_position(std::make_unique<glm::vec3>())
{
}
ClothMass::ClothMass(const glm::vec3 &position) : m_mass(1.f), m_position(std::make_unique<glm::vec3>(position))
{
}
ClothMass::ClothMass(float mass, const glm::vec3 &position)
    : m_mass(mass), m_position(std::make_unique<glm::vec3>(position))
{
}

float ClothMass::getMass()
{
    return m_mass;
}

const glm::vec3 &ClothMass::getPosition()
{
    return *m_position;
}

void ClothMass::setMass(float mass)
{
    m_mass = mass;
}
void ClothMass::setPosition(const glm::vec3 &position)
{
    m_position = std::make_unique<glm::vec3>(position);
}

void ClothMass::addForce(const glm::vec3 &force)
{
    *m_acceleration += force / m_mass;
}

void ClothMass::setFixed(bool fixed)
{
    m_isFixed = fixed;
}

void ClothMass::offsetPosition(const glm::vec3 &offset)
{
    *m_position += offset;
}

void ClothMass::step(float dt, float damping)
{
    if (!m_isFixed)
    {
        glm::vec3 temp = *m_position;
        *m_position = *m_position + (*m_position - *m_oldPosition) * (1.f - damping) + *m_acceleration * dt;
        *m_oldPosition = temp;
        *m_acceleration = glm::vec3(0.f);
    }
}
