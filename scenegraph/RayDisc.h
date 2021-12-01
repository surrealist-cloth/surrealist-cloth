#ifndef RAYDISC_H
#define RAYDISC_H
#include <glm/glm.hpp>
#include <iostream>
#include <vector> // vec


class RayDisc
{
public:
    RayDisc();

    // just works with x and z values
    static std::vector<float> intersect(glm::vec4 &p, glm::vec4 &d);
};

#endif // RAYDISC_H
