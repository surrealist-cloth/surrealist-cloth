#include "RayCube.h"
#include <vector>
#include<algorithm>

#include "scenegraph/Constants.h"


RayCube::RayCube()
{

}

float RayCube::intersects(glm::vec4 &p, glm::vec4 &d) {

    assert(-1.0f <= d.x <= 1.0f); //check dir roughly normalized
    assert(-1.0f <= d.y <= 1.0f);
    assert(-1.0f <= d.z <= 1.0f);

    // checking if ray intersects face A (z = 0.5) // should see
    if ( !(d.z == 0.0f) ) { // dir.z not 0, can calculate t and dir.z is relevant

        float t = (0.5f - p.z ) / d.z;

        if (t > 0.0f) { // ray hits z plane (without going backwards)

            float zPlaneHit_Y = p.y + (d.y * t);
            float zPlaneHit_X = p.x + (d.x * t);

            if (-0.5f <=  zPlaneHit_Y &&  zPlaneHit_Y <= 0.5f) { // point is within face bounds in y
                if (-0.5f <= zPlaneHit_X && zPlaneHit_X <= 0.5f) { // point is within bounds in x
                    return t; // ray intersects face A (z = 0.5)
                }

            }
        }
    }

    // checking if ray intersects face B (x = -0.5) // no see
    if ( !(d.x == 0.0f) ) { // dir.z not 0, can calculate t and dir.z is relevant

        float t = ((-0.5f) - p.x ) / d.x;

        if (t > 0.0f) { // ray hits z plane (without going backwards)

            float xPlaneHit_Y = p.y + (d.y * t);
            float xPlaneHit_Z = p.z + (d.z * t);

            if (-0.5f <=  xPlaneHit_Y &&  xPlaneHit_Y <= 0.5f) { // point is within face bounds in y
                if (-0.5f <= xPlaneHit_Z && xPlaneHit_Z <= 0.5f) { // point is within bounds in x
                    return t; // ray intersects face B (x = -0.5)
                }

            }
        }
    }

    // checking if ray intersects face C (y = 0.5) // should see
    if ( !(d.y == 0.0f) ) { // dir.z not 0, can calculate t and dir.z is relevant

        float t = (0.5f - p.y ) / d.y;

        if (t > 0.0f) { // ray hits z plane (without going backwards)

            float yPlaneHit_Z = p.z + (d.z * t);
            float yPlaneHit_X = p.x + (d.x * t);

            if (-0.5f <=  yPlaneHit_Z &&  yPlaneHit_Z <= 0.5f) { // point is within face bounds in y
                if (-0.5f <= yPlaneHit_X && yPlaneHit_X <= 0.5f) { // point is within bounds in x
                    return t; // ray hits z plane
                }

            }
        }
    }

    // checking if ray intersects face D (z = -0.5) // no see
    if ( !(d.z == 0.0f) ) { // dir.z not 0, can calculate t and dir.z is relevant

        float t = ((-0.5f) - p.z ) / d.z;

        if (t > 0.0f) { // ray hits z plane (without going backwards)

            float zPlaneHit_Y = p.y + (d.y * t);
            if (-0.5f <=  zPlaneHit_Y &&  zPlaneHit_Y <= 0.5f) { // point is within face bounds in y

                float zPlaneHit_X = p.x + (d.x * t);
                if (-0.5f <= zPlaneHit_X && zPlaneHit_X <= 0.5f) { // point is within bounds in x

                    return t; // ray hits z plane
                }

            }
        }
    }

    // checking if ray intersects face E (x = 0.5) // shoudl see
    if ( !(d.x == 0.0f) ) { // dir.z not 0, can calculate t and dir.z is relevant

        float t = (0.5f - p.x) / d.x;

        if (t > 0.0f) { // ray hits z plane (without going backwards)

            float xPlaneHit_Y = p.y + (d.y * t);
            if (-0.5f <=  xPlaneHit_Y &&  xPlaneHit_Y <= 0.5f) { // point is within face bounds in y

                float xPlaneHit_Z = p.z + (d.z * t);
                if (-0.5f <= xPlaneHit_Z && xPlaneHit_Z <= 0.5f) { // point is within bounds in x

                    return t; // ray intersects face B (x = -0.5)
                }

            }
        }
    }

    // checking if ray intersects face F (y = -0.5) // no see
    if ( !(d.y == 0.0f) ) { // dir.z not 0, can calculate t and dir.z is relevant

        float t = ((-0.5f) - p.y ) / d.y;

        if (t > 0.0f) { // ray hits z plane (without going backwards)

            float yPlaneHit_Z = p.z + (d.z * t);
            if (-0.5f <=  yPlaneHit_Z &&  yPlaneHit_Z <= 0.5f) { // point is within face bounds in y

                float yPlaneHit_X = p.x + (d.x * t);
                if (-0.5f <= yPlaneHit_X && yPlaneHit_X <= 0.5f) { // point is within bounds in x

                    return t; // ray hits z plane
                }
            }
        }
    }

    return 0;
}


/*
 * p is a point on the surface of the sphere in object space.
**/
glm::vec4 RayCube::getNormal(glm::vec4 &p) {

    float l = CUBE_LENGTH / 2.0f;

//    if (abs(p.y - l) < EPSILON) {
//    }

    if (l - EPSILON < p.x && p.x < l + EPSILON) {
        return glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    }

    if (-l - EPSILON < p.x && p.x < -l + EPSILON) {
        return glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);
    }

    if (l - EPSILON < p.y && p.y < l + EPSILON) {
        return glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    }

    if (-l - EPSILON < p.y && p.y < -l + EPSILON) {
        return glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
    }

    if (l - EPSILON < p.z && p.z < l + EPSILON) {
        return glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    }

    if (-l - EPSILON < p.z && p.z < -l + EPSILON) {
        return glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    }

    return glm::vec4();
}
