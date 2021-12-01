#ifndef RAYCYLINDER_H
#define RAYCYLINDER_H
#include <glm/glm.hpp> //vecs
#include <vector>

class RayCylinder
{
public:
    RayCylinder();
    static float intersects(glm::vec4 &p, glm::vec4 &d);
    static glm::vec4 getNormal(glm::vec4 &p);
private:
    static float intersectsDisk(glm::vec4 &p, glm::vec4 &d);
    static float intersectsConeBase(glm::vec4 &p, glm::vec4 &d);
};

#endif // RAYCYLINDER_H
