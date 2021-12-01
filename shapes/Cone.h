#ifndef CONE_H
#define CONE_H

#include "OpenGLShape.h"
#include "TriangleStrip.h"
#include "Rectangle.h"



class Cone : public OpenGLShape
{
public:
    Cone();
    Cone(int param1, int param2);
    Cone(int param1, int param2, glm::mat4 transform);

    ~Cone();


protected:
    void build();

private:
    int m_param1;
    int m_param2;
    std::unique_ptr<TriangleStrip> m_triangleStrip;
    glm::mat4 m_transform;

};

#endif // CONE_H
