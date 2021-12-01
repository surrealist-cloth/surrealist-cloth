#ifndef CUBE_H
#define CUBE_H

#include "OpenGLShape.h"
#include "TriangleStrip.h"
#include "Rectangle.h"


class Cube : public OpenGLShape
{
public:
    Cube();
    Cube(int param1, int param2);
    Cube(int param1, int param2, glm::mat4 transform); // transform is a transformation matrix
    // that will be applied to all the shapes.
    ~Cube();

    //returns 0 if it does not intersect, the value of t if it does!!
    static float intersects(glm::vec4 &p, glm::vec4 &d);
protected:
    void build();

private:
    int m_param1;
    int m_param2;
    std::unique_ptr<TriangleStrip> m_triangleStrip;
    glm::mat4 m_transform;
};

#endif // CUBE_H
