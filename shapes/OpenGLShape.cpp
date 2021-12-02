#include "OpenGLShape.h"
#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include "Triangle.h"
#include <vector>
#include <iostream>
#include <memory>
using namespace CS123::GL;

OpenGLShape::OpenGLShape() :
    m_shape(nullptr),
    m_VAO(nullptr)
{

}

OpenGLShape::~OpenGLShape()
{
}

void OpenGLShape::draw() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

void OpenGLShape::loadShape(std::unique_ptr<Shape> shape) {
    m_shape = std::move(shape);

    std::vector<Triangle> triangles = m_shape->getTrianglesRecursive();
    m_vertexData.clear();
    m_vertexData.reserve(triangles.size() * 6);

    for (int i = 0; i < triangles.size(); i++) {
        insertVec3(m_vertexData, triangles[i].a());
        insertVec3(m_vertexData, triangles[i].aNormal());
        insertVec3(m_vertexData, triangles[i].b());
        insertVec3(m_vertexData, triangles[i].bNormal());
        insertVec3(m_vertexData, triangles[i].c());
        insertVec3(m_vertexData, triangles[i].cNormal());
    }

    initializeOpenGLShapeProperties();
}

bool OpenGLShape::isLoaded()
{
    return !!m_shape;
}

void OpenGLShape::initializeOpenGLShapeProperties() {
    const int numFloatsPerVertex = 6;
    const int numVertices = m_vertexData.size() / numFloatsPerVertex;

    std::vector<VBOAttribMarker> markers;
    markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));
    VBO vbo = VBO(m_vertexData.data(), m_vertexData.size(), markers);
    m_VAO = std::make_unique<VAO>(vbo, numVertices);
}
