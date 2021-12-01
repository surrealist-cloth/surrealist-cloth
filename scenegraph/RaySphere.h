#ifndef RAYSPHERE_H
#define RAYSPHERE_H
#include <glm/glm.hpp> //vecs
#include <vector>

class RaySphere
{
public:
    RaySphere();
    static float intersects(glm::vec4 &p, glm::vec4 &d);
    static std::vector<float> getIntersections(glm::vec4 &p, glm::vec4 &d);
    static glm::vec4 getNormal(glm::vec4 &p);
};

#endif // RAYSPHERE_H
