#include "RayInfCone.h"
#include "RayCylinder.h"
#include <vector>
#include<algorithm>

#include "scenegraph/Constants.h"


RayCylinder::RayCylinder()
{

}

/*
 * p is a point on the surface of the sphere in object space.
**/
glm::vec4 RayCylinder::getNormal(glm::vec4 &p) {

    float r = CYLINDER_RADIUS;
    float h = CYLINDER_HEIGHT / 2.0f;

    if (h - EPSILON <= p.y && p.y <= h + EPSILON) {
        return glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    }

    if (- h - EPSILON <= p.y && p.y <= - h + EPSILON) {
        return glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
    }

    if ( - h + EPSILON <= p.y && p.y <= h - EPSILON ) {
        return glm::normalize(glm::vec4(2.0f * p.x , 0.0f, 2.0f * p.z, 0.0f));
    }

    return glm::normalize(  glm::vec4(2.0f * p.x,
                                      0.0f,
                                      2.0f * p.z,
                                      0.0f)
                            );
}

float RayCylinder::intersects(glm::vec4 &p, glm::vec4 &d) {

    float h = (CYLINDER_HEIGHT/2.0f); //height of cylinder/2
    float r = CYLINDER_RADIUS; //radius of cylinder

    std::vector<float> t_stack;
    t_stack.reserve(4);
    std::vector<float> t_cylinder = RayInfCone::intersect(p, d);

    float y_0 = p.y + d.y * t_cylinder[0];
    float y_1 = p.y + d.y * t_cylinder[1];

    if (!(y_0 > h || y_0 < - h)) {
        t_stack.push_back(t_cylinder[0]);
    }

    if (!(y_1 > h || y_1 < - h)) {
        t_stack.push_back(t_cylinder[1]);
    }

    float t3 = ((0.5f) - p.y ) / d.y;

    if (pow(p.x + d.x * t3, 2) + pow(p.z + d.z * t3, 2) <= pow(r ,2)) {
        t_stack.push_back(t3);
    }

    float t4 = ((-0.5f) - p.y ) / d.y;

    if (pow(p.x + d.x * t4, 2) + pow(p.z + d.z * t4, 2) <= pow(r ,2)) {
        t_stack.push_back(t4);
    }

    //remove 0s
    t_stack.erase( std::remove( t_stack.begin(), t_stack.end(), 0.0f ),
                   t_stack.end() );

    if (t_stack.size() >= 1) {
        return *min_element(t_stack.begin(), t_stack.end());
    }

    return 0.0f;
}
