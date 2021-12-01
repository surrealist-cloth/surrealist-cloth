#ifndef MYMATH_H
#define MYMATH_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform2.hpp>
#include "glm/ext.hpp"

/**
 * @brief The MyMath class is a wrapper for certain math operations
 * notably vector and matrix multiplication.
 */
class MyMath
{
public:
    MyMath();

    static glm::vec4 multiply(glm::mat4x4 m, glm::vec4 p);
    static glm::vec4 multiply(glm::vec4 p, glm::mat4x4 m);
};

#endif // MYMATH_H
