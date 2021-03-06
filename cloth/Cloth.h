#ifndef CLOTH_H
#define CLOTH_H

#include <memory>
#include <vector>

#include "ClothConstraint.h"
#include "ClothMass.h"
#include "ishapes/MeshIShape.h"

class Cloth
{
  public:
    Cloth(int rows, int cols, float timeStep = 0.02, float width = 10, float height = 10);
    ~Cloth();
    void step();
    void translate(const glm::vec3 &direction);
    void addForce(const glm::vec3 &force);
    void addWindForce(const glm::vec3 &wind);
    void avoidSelfCollisions();
    void toObj(const std::string &filename);
    ClothMass &massAt(int row, int col);
    void translateMass(ClothMass &mass, const glm::vec3 &translation);
    MeshIShape toMesh() const;

  protected:
    int m_rows;
    int m_cols;
    std::vector<std::unique_ptr<ClothMass>> m_masses;
    std::vector<std::unique_ptr<ClothConstraint>> m_constraints;

    inline std::vector<glm::vec3> getVertices() const;
    inline std::vector<Tri> getFaces() const;

    // constants
    float m_timeStep;
    float m_width;
    float m_height;
    const float m_stiffness = 0.2f;
    const float m_damping = 0.01f;
    const int m_constraintIterations = 15;
    const float m_selfCollisionDistance = 0.05f;
};

#endif // CLOTH_H
