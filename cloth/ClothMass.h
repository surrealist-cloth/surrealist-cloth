#ifndef CLOTHMASS_H
#define CLOTHMASS_H

#include <glm/glm.hpp>
#include <memory>

class ClothMass
{
  public:
    ClothMass();
    ClothMass(float mass);
    ClothMass(const glm::vec3 &position);
    ClothMass(float mass, const glm::vec3 &position);
    ~ClothMass();
    float getMass();
    const glm::vec3 &getPosition();
    void setMass(float mass);
    void setPosition(const glm::vec3 &position);
    void offsetPosition(const glm::vec3 &offset);
    void addForce(const glm::vec3 &force);
    void setFixed(bool fixed);
    void translate(const glm::vec3 &offset);
    void step(float dt, float damping);

  protected:
    bool m_isFixed;
    float m_mass;
    std::unique_ptr<glm::vec3> m_position;
    std::unique_ptr<glm::vec3> m_oldPosition;
    std::unique_ptr<glm::vec3> m_acceleration;
};

#endif // CLOTHMASS_H
