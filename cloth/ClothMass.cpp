#include "ClothMass.h"

ClothMass::~ClothMass() {

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
