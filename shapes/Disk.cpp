#include "Disk.h"

Disk::Disk(int num_cols, int num_rows,
                         glm::mat4 transformPositions, glm::mat4 transformNormals) :
    Mesh(num_cols, num_rows, transformPositions, transformNormals)
{
}

Disk::Disk(int num_cols, int num_rows,
                         glm::mat4 transformPositions) :
    Mesh(num_cols, num_rows, transformPositions)
{
}

Disk::Disk(int num_cols, int num_rows) :
    Mesh(num_cols, num_rows)
{
}

std::vector<GLfloat> Disk::getPointData(int col, int row) {

    float x = (0.5 * static_cast<float>(row)/getNumRows())
            * cos( ((static_cast<float>(col)/getNumCols()) * 2 * -3.1415));

    float y = (0.5 * static_cast<float>(row)/getNumRows())
            * sin( ((static_cast<float>(col)/getNumCols()) * 2 * -3.1415));

    glm::vec4 position = getTransformPositions() * glm::vec4(x, y, 0.0f, 1.0f);

    glm::vec4 normal = getTransformNormals() * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

    return {position.x, position.y, position.z,
            normal.x, normal.y, normal.z};
}

Disk::~Disk()
{
}
