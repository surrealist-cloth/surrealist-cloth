#include "Cube.h"
#include "TriangleStrip.h"

#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"
#include <iostream>

Cube::Cube()
{
}


Cube::Cube(int param1, int param2):
    Cube(param1, param2, glm::mat4(1.0f))
{
}

Cube::Cube(int param1, int param2, glm::mat4 transform) :
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


void Cube::build() {

    glm::mat4x4 translateFaceA = glm::translate(glm::vec3(-0.5f, -0.5f, 0.5f));
    glm::mat4x4 rotateFaceB = glm::rotate(-3.1415f/2, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4x4 rotateFaceC = glm::rotate(-3.1415f/2, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4x4 rotateFaceD = glm::rotate(3.1415f, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4x4 rotateFaceE = glm::rotate(3.1415f/2, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4x4 rotateFaceF = glm::rotate(3.1415f/2, glm::vec3(1.0f, 0.0f, 0.0f));

    Rectangle faceA =
            Rectangle(m_param1, m_param1, m_transform * translateFaceA);
    Rectangle faceB =
            Rectangle(m_param1, m_param1, m_transform * rotateFaceB * translateFaceA, rotateFaceB);
    Rectangle faceC =
            Rectangle(m_param1, m_param1, m_transform * rotateFaceC * translateFaceA, rotateFaceC);

    Rectangle faceD =
            Rectangle(m_param1, m_param1, m_transform * rotateFaceD * translateFaceA, rotateFaceD);
    Rectangle faceE =
            Rectangle(m_param1, m_param1, m_transform * rotateFaceE * translateFaceA, rotateFaceE);
    Rectangle faceF =
            Rectangle(m_param1, m_param1, m_transform * rotateFaceF * translateFaceA, rotateFaceF);

    m_vertexData = m_triangleStrip->buildTriangleStrip(m_param1, m_param1, faceA);
    m_triangleStrip->addTriangleStripTo(&m_vertexData, m_param1, m_param1, faceB);
    m_triangleStrip->addTriangleStripTo(&m_vertexData, m_param1, m_param1, faceC);

    m_triangleStrip->addTriangleStripTo(&m_vertexData, m_param1, m_param1, faceD);
    m_triangleStrip->addTriangleStripTo(&m_vertexData, m_param1, m_param1, faceE);
    m_triangleStrip->addTriangleStripTo(&m_vertexData, m_param1, m_param1, faceF);
}

Cube::~Cube() {
}
