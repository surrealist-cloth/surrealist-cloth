#include "RayDisc.h"

#include <iostream>
#include <vector> // vec

RayDisc::RayDisc()
{

}


// just works with x and z values
std::vector<float> RayDisc::intersect(glm::vec4 &p, glm::vec4 &d) {

    std::vector<float> t_stack;
    t_stack.reserve(2);

    assert(!(d.x == 0.0f));
    assert(!(d.z == 0.0f));

    float r = .5f; // radius;

    float a = pow(d.x, 2) + pow(d.z, 2); //quadratic terms
    float b = 2.0f * p.x * d.x + 2.0f * p.z * d.z;
    float c = pow(p.x, 2) + pow(p.z, 2) - pow(r, 2);

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
