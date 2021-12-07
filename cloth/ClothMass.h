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

  protected:
    float m_mass;
    std::unique_ptr<glm::vec3> m_position;
};

#endif // CLOTHMASS_H
