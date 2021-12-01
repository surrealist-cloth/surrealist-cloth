#include "MyMath.h"

MyMath::MyMath()
{

}


glm::vec4 MyMath::multiply(glm::mat4x4 m, glm::vec4 p) {
    return multiply(p, m);
}

glm::vec4 MyMath::multiply(glm::vec4 p, glm::mat4x4 m) {

    return glm::vec4( m[0][0] * p[0]   +   m[0][1] * p[1]   +   m[0][2] * p[2]   +   m[0][3] * p[3],
                      m[1][0] * p[0]   +   m[1][1] * p[1]   +   m[1][2] * p[2]   +   m[1][3] * p[3],
                      m[2][0] * p[0]   +   m[2][1] * p[1]   +   m[2][2] * p[2]   +   m[2][3] * p[3],
                      m[3][0] * p[0]   +   m[3][1] * p[1]   +   m[3][2] * p[2]   +   m[3][3] * p[3]
                    );
}
