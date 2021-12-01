#ifndef RAYCONE_H
#define RAYCONE_H


#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"

class RayCone
{
public:
    RayCone();
    //returns 0 if it does not intersect, the value of t if it does
    static float intersects(glm::vec4 &p, glm::vec4 &d);
    static glm::vec4 getNormal(glm::vec4 &p);

private:
    static float intersectsConeShell(glm::vec4 &p, glm::vec4 &d);
    static float intersectsConeBase(glm::vec4 &p, glm::vec4 &d);
};

#endif // RAYCONE_H
