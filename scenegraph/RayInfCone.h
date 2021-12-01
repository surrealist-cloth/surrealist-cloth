#ifndef RAYINFCONE_H
#define RAYINFCONE_H
#include <glm/glm.hpp>
#include <iostream>
#include <vector> // vec


class RayInfCone
{
public:
    RayInfCone();

    // just works with x and z values
    static std::vector<float> intersect(glm::vec4 &p, glm::vec4 &d);
};

#endif // RAYINFCONE_H
