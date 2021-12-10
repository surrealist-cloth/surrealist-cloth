#ifndef CLOTH_H
#define CLOTH_H

#include <memory>
#include <vector>

#include "ClothConstraint.h"
#include "ClothMass.h"

class Cloth
{
  public:
    Cloth(int rows, int cols);
    ~Cloth();
    void step();
    void translate(const glm::vec3 &direction);
    void addForce(const glm::vec3 &force);
    void addWindForce(const glm::vec3 &wind);
    void avoidSelfCollisions();

  protected:
    int m_rows;
    int m_cols;
    std::vector<std::unique_ptr<ClothMass>> m_masses;
    std::vector<std::unique_ptr<ClothConstraint>> m_constraints;
    // constants
    const float m_width = 4.f;
    const float m_height = 4.f;
    const float m_stiffness = 0.1;
    const float m_timeStep = 1.f / 60.f;
    const float m_damping = 0.1;
    const int m_constraintIterations = 10;
    const float m_selfCollisionDistance = 0.1;
};

#endif // CLOTH_H
