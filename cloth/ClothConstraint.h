#ifndef CLOTHCONSTRAINT_H
#define CLOTHCONSTRAINT_H

#include "ClothMass.h"

class ClothConstraint
{
  public:
    ClothConstraint(ClothMass &mass1, ClothMass &mass2, float stiffness);
    ~ClothConstraint();
    void satisfyConstraint();
    ClothMass &getMass1();
    ClothMass &getMass2();

  protected:
    ClothMass &m_mass1;
    ClothMass &m_mass2;
    float m_stiffness;
    float m_equilibriumDistance;
};

#endif // CLOTHCONSTRAINT_H
