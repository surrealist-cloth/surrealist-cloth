#ifndef CYLINDER_H
#define CYLINDER_H

#include "OpenGLShape.h"
#include "TriangleStrip.h"
#include "Rectangle.h"


class Cylinder : public OpenGLShape
{
public:
    Cylinder();
    Cylinder(int param1, int param2);
    Cylinder(int param1, int param2, glm::mat4 transform);
    ~Cylinder();
protected:
    void build();

private:
    int m_param1;
    int m_param2;
    std::unique_ptr<TriangleStrip> m_triangleStrip;
    glm::mat4 m_transform;

};

#endif // CYLINDER_H
