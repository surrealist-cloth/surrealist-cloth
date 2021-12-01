#include "Cylinder.h"
#include "TriangleStrip.h"
#include "Disk.h"
#include "Tube.h"

#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"


Cylinder::Cylinder()
{
}

Cylinder::Cylinder(int param1, int param2):
    Cylinder(param1, param2, glm::mat4(1.0f))
{
}


Cylinder::Cylinder(int param1, int param2, glm::mat4 transform) :
    m_param1(param1),
    m_param2(param2),
    m_triangleStrip(std::make_unique<TriangleStrip>() ),
    m_transform(glm::mat4(transform))
{

    build();


    /**
     * Initialize the OpenGLShapeProperties so that the shape is ready to be drawn.
     * Note that this does not actually draw the shape, that happens in ShapesScene.cpp
     */
    initializeOpenGLShapeProperties();
}

void Cylinder::build() {

    if (m_param1 < 4) {
        m_param1 = 3;
    }

    glm::mat4 translateZ = glm::translate(glm::vec3(0, 0, 0.5f));
    glm::mat4 translateZBack = glm::translate(glm::vec3(0, 0, -0.5f));
    glm::mat4 rotateX = glm::rotate(3.1415f/2, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 rotateXBack = glm::rotate(-3.1415f/2, glm::vec3(1.0f, 0.0f, 0.0f));


    Disk topCap =
            Disk(m_param1, m_param2, m_transform * rotateXBack * translateZ, rotateXBack);
    Disk bottomCap =
            Disk(m_param1, m_param2, m_transform * rotateX * translateZ, rotateX);
    Tube tube =
            Tube(m_param1, m_param2,  m_transform * rotateX * translateZBack, rotateX);

    m_triangleStrip->addTriangleStripTo(&m_vertexData, m_param1, m_param2, topCap);
    m_triangleStrip->addTriangleStripTo(&m_vertexData, m_param1, m_param2, bottomCap);
    m_triangleStrip->addTriangleStripTo(&m_vertexData, m_param1, m_param2, tube);
}

Cylinder::~Cylinder()
{
}
