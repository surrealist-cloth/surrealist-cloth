#ifndef CLOTHCONSTRAINT_H
#define CLOTHCONSTRAINT_H

#include "ClothMass.h"

class ClothConstraint
{
  public:
    ClothConstraint(ClothMass &mass1, ClothMass &mass2, float stiffness);
    ~ClothConstraint();
    void satisfyConstraint();

  protected:
    float m_equilibriumDistance;
    float m_stiffness;
    ClothMass &m_mass1;
    ClothMass &m_mass2;
};

#endif // CLOTHCONSTRAINT_H
