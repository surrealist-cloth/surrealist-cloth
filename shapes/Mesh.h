#ifndef MESH_H
#define MESH_H

#include "GL/glew.h"
#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"

#include<memory>
#include <vector>

class Mesh
{
public:
    Mesh(int num_cols, int num_rows,
                             glm::mat4 transformPositions, glm::mat4 transformNormals);
    Mesh(int numCols, int numRows, glm::mat4 transformPositions);
    Mesh(int numCols, int numRows);

    virtual std::vector<GLfloat> getPointData(int col, int row) = 0;

    int getNumCols();
    int getNumRows();
    glm::mat4 getTransformPositions();
    glm::mat4 getTransformNormals();

    void setNumCols(int numCols);
    void setNumRows(int numRows);
    void setTransformPositions(glm::mat4 transformPositions);
    void setTransformNormals(glm::mat4 transformNormals);

    ~Mesh();
private:
    float m_numCols;
    float m_numRows;
    glm::mat4 m_transformPositions;
    glm::mat4 m_transformNormals;
};

#endif // MESH_H
