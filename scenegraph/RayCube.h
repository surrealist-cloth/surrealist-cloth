#ifndef RAYCUBE_H
#define RAYCUBE_H
#include <glm/glm.hpp> //vecs
#include <vector>

class RayCube
{
public:
    RayCube();
    static float intersects(glm::vec4 &p, glm::vec4 &d);
    static glm::vec4 getNormal(glm::vec4 &p);
};

#endif // RAYCUBE_H
