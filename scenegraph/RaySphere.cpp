#include "RaySphere.h"
#include <vector>

#include<algorithm>

RaySphere::RaySphere()
{

}

float RaySphere::intersects(glm::vec4 &p, glm::vec4 &d) {
    float height = .5f; //height of cylinder

    std::vector<float> t_stack;
    t_stack.reserve(4);
    std::vector<float> t_cylinder = getIntersections(p, d);

    float y_0 = p.y + d.y * t_cylinder[0];
    float y_1 = p.y + d.y * t_cylinder[1];

    if (!(y_0 > height || y_0 < -height)) {
        t_stack.push_back(t_cylinder[0]);
    }

    if (!(y_1 > height || y_1 < -height)) {
        t_stack.push_back(t_cylinder[1]);
    }

    //remove 0s
    t_stack.erase( std::remove( t_stack.begin(), t_stack.end(), 0.0f ),
                   t_stack.end() );

    if (t_stack.size() >= 1) {
        return *min_element(t_stack.begin(), t_stack.end());
    }

    return 0.0f;
}

/*
 * p is a point on the surface of the sphere in object space.
**/
glm::vec4 RaySphere::getNormal(glm::vec4 &p) {
    return glm::normalize(  glm::vec4(2.0f * p.x,
                                      2.0f * p.y,
                                      2.0f * p.z,
                                      0.0f)
                            );
}

std::vector<float> RaySphere::getIntersections(glm::vec4 &p, glm::vec4 &d) {
    std::vector<float> t_stack;
    t_stack.reserve(2);

    assert(!(d.x == 0.0f && d.z == 0.0f && d.y == 0.0f));

    float r = .5f; // radius;

    float a = pow(d.x, 2) + pow(d.y, 2) + pow(d.z, 2); //quadratic terms
    float b = 2.0f * p.x * d.x + 2.0f * p.y * d.y + 2.0f * p.z * d.z;
    float c = pow(p.x, 2) + pow(p.y, 2) + pow(p.z, 2) - pow(r, 2);

    float disc = pow(b, 2) - 4.0f * a * c; // discriminant

    if (disc < 0) {
        t_stack.push_back(0.0f);
        t_stack.push_back(0.0f);

    } else if (disc == 0) {
        t_stack.push_back(-b/ (2.0f * a));
        t_stack.push_back(0.0f);

    } else if (disc > 0) {
        t_stack.push_back((-b + sqrt(disc)) / (2.0f * a));
        t_stack.push_back((-b - sqrt(disc)) / (2.0f * a));
    }

    return t_stack;
}
