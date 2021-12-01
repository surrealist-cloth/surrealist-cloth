#ifndef SPHERE_H
#define SPHERE_H

#include "OpenGLShape.h"
#include "TriangleStrip.h"
#include "Rectangle.h"


class Sphere : public OpenGLShape
{
public:
    Sphere();
    Sphere(int param1, int param2);
    Sphere(int param1, int param2, glm::mat4 transform);
    ~Sphere();

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

#endif // SPHERE_H
