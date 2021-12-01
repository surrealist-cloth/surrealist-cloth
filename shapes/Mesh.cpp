#include "Mesh.h"
#include <iostream>

Mesh::Mesh(int num_cols, int num_rows,
                         glm::mat4 transformPositions, glm::mat4 transformNormals) :
    m_numCols(num_cols),
    m_numRows(num_rows),
    m_transformPositions(transformPositions),
    m_transformNormals(transformNormals)
{
}

Mesh::Mesh(int num_cols, int num_rows,
                         glm::mat4 transformPositions) :
    m_numCols(num_cols),
    m_numRows(num_rows),
    m_transformPositions(transformPositions),
    m_transformNormals(glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)))
{
}

Mesh::Mesh(int num_cols, int num_rows) :
    m_numCols(num_cols),
    m_numRows(num_rows),
    m_transformPositions(glm::translate(glm::vec3(0.0f, 0.0f, 0.0f))),
    m_transformNormals(glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)))
{
}


//std::vector<GLfloat> Mesh::getPointData(int col, int row) {
//    std::cout << "BAD GETPOINTDATA CALLED IN MESH" << std::endl;
//    return std::vector<GLfloat> (col, row);
//}

int Mesh::getNumCols(){
    return m_numCols;
}

int Mesh::getNumRows(){
    return m_numRows;
}

glm::mat4 Mesh::getTransformPositions(){
    return m_transformPositions;
}

glm::mat4 Mesh::getTransformNormals(){
    return m_transformNormals;
}


void Mesh::setNumCols(int numCols) {
    m_numCols = numCols;
}

void Mesh::setNumRows(int numRows) {
    m_numRows = numRows;
}

void Mesh::setTransformPositions(glm::mat4 transformPositions) {
    m_transformPositions = transformPositions;
}

void Mesh::setTransformNormals(glm::mat4 transformNormals) {
    m_transformNormals = transformNormals;
}

Mesh::~Mesh()
{
}
