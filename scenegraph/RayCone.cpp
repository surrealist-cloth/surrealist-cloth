#include "RayCone.h"

#include <cmath>
#include <math.h> //  sqrt
#include <algorithm>  // std::min

#include "scenegraph/Constants.h"

RayCone::RayCone()
{

}


float RayCone::intersects(glm::vec4 &p, glm::vec4 &d) {

    if ( !(intersectsConeShell(p, d) == 0) ) {
        return intersectsConeShell(p, d);
    }

    if (!(intersectsConeBase(p, d) == 0)) { // You need to compare the t's between these 2
        return intersectsConeBase(p,d);
    }

    return 0;
}


/*
 * p is a point on the surface of the sphere in object space.
**/
glm::vec4 RayCone::getNormal(glm::vec4 &p) {
    float h = CYLINDER_HEIGHT / 2.0f;

    if (abs(-h - p.y) < EPSILON) {
        return glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
    }
    return glm::normalize(  glm::vec4(2.0f * p.x,
                                      -0.5f * (0.5f - p.y),
                                      2.0f * p.z,
                                      0.0f)
                            );
}








float RayCone::intersectsConeBase(glm::vec4 &p, glm::vec4 &d) {
    if (! (d.y == 0.0f)) {
        float t = ((-0.5f) - p.y ) / d.y;

        if (t > 0.0f) {
            if (d.y > 0 && d.x > 0) {
                float yPlaneHit_Z = p.z + (d.z * t);
                float yPlaneHit_X = p.x + (d.x * t);

                float poly = pow(yPlaneHit_Z, 2) + pow(yPlaneHit_X, 2);

                if ( 0.25f >=  poly ) {
                    return t;
                }
                return 0;
            }
        }
    }
    return 0;
}


float RayCone::intersectsConeShell(glm::vec4 &p, glm::vec4 &d) {

    float a = (pow(d.x, 2) + pow(d.z, 2) - (1.0f/4.0f) * pow(d.y,2));
    float b = (2 * p.x * d.x +  2 * p.z * d.z - (1.0f/2.0f) * p.y * d.y + (1.0f/4.0f) * d.y);
    float c = (pow(p.x, 2) + pow(p.z, 2) - (1.0f/4.0f) * pow(p.y, 2) + (1.0f/4.0f) * p.y - (1.0f/16.0f));

    float disc = pow(b, 2) - 4 * a * c;

    if (a == 0) {
        return -c/b;
    }

    if ( disc >= 0.0f ) {
        float t_1;

        if (disc == 0.0f  && t_1 > 0.0f) {
            t_1 = -b/(2 * a);

            if ( -0.5f <= (p.y + d.y * t_1)  &&  (p.y + d.y * t_1) <= 0.5f ) {
               return t_1;
            }
        }

        t_1 = (-b + sqrt(disc)) / (2 * a);
        float t_2 = (-b - sqrt(disc)) / (2 * a);

        if (t_1 > 0.0f && t_2 < 0.0f) {
            if ( -0.5f <= (p.y + d.y * t_1)  &&  (p.y + d.y * t_1) <= 0.5f ) {
                return t_1;
            }
        }

        if (t_2 > 0.0f && t_1 < 0.0f) {
            if ( -0.5f <= (p.y + d.y * t_2)  &&  (p.y + d.y * t_2) <= 0.5f ) {
                return t_2;
            }
        }

        if (t_2 > 0.0f && t_1 > 0.0f) {

            if (t_2 < t_1) {
                if ( -0.5f <= (p.y + d.y * t_2)  &&  (p.y + d.y * t_2) <= 0.5f ) {
                    return t_2;
                }
            } else if (t_1 < t_2) {
                if ( -0.5f <= (p.y + d.y * t_1)  &&  (p.y + d.y * t_1) <= 0.5f ) {
                    return t_1;
                }
            }

        }
    }
        return 0;
}
