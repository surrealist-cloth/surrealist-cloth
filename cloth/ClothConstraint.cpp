#include "ClothConstraint.h"

ClothConstraint::ClothConstraint(ClothMass &mass1, ClothMass &mass2, float stiffness)
    : m_mass1(mass1), m_mass2(mass2), m_stiffness(stiffness)
{
}