#include "Rectangle.h"
#include <iostream>

Rectangle::Rectangle(int num_cols, int num_rows,
                         glm::mat4 transformPositions, glm::mat4 transformNormals) :
    Mesh(num_cols, num_rows, transformPositions, transformNormals)
{
}

Rectangle::Rectangle(int num_cols, int num_rows,
                         glm::mat4 transformPositions) :
    Mesh(num_cols, num_rows, transformPositions)
{
}

Rectangle::Rectangle(int num_cols, int num_rows) :
    Mesh(num_cols, num_rows)
{
}

std::vector<GLfloat> Rectangle::getPointData(int col, int row) {

    glm::vec4 position = getTransformPositions() *
            glm::vec4(static_cast<float>(col)/getNumCols(), static_cast<float>(row)/getNumRows(), 0.0f, 1.0f);
    glm::vec4 normal = getTransformNormals() * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

    return {position.x, position.y, position.z,
            normal.x, normal.y, normal.z};
}


Rectangle::~Rectangle()
{
}
