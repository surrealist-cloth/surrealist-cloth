#include "ClothConstraint.h"

ClothConstraint::~ClothConstraint()
{
}

ClothConstraint::ClothConstraint(ClothMass &mass1, ClothMass &mass2, float stiffness)
    : m_mass1(mass1), m_mass2(mass2), m_stiffness(stiffness),
      m_equilibriumDistance(glm::length(mass1.getPosition() - mass2.getPosition()))
{
}

void ClothConstraint::satisfyConstraint()
{
    glm::vec3 direction = m_mass2.getPosition() - m_mass1.getPosition();
    float distance = glm::length(direction);
    glm::vec3 correctionVector = direction * (1.0f - m_equilibriumDistance / distance) * 0.5f;
    m_mass1.offsetPosition(correctionVector);
    m_mass2.offsetPosition(-correctionVector);
}

ClothMass &ClothConstraint::getMass1()
{
    return m_mass1;
}

ClothMass &ClothConstraint::getMass2()
{
    return m_mass2;
}