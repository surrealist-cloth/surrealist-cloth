#include "Tube.h"

Tube::Tube(int num_cols, int num_rows,
                         glm::mat4 transformPositions, glm::mat4 transformNormals) :
    Mesh(num_cols, num_rows, transformPositions, transformNormals)
{
}

Tube::Tube(int num_cols, int num_rows,
                         glm::mat4 transformPositions) :
    Mesh(num_cols, num_rows, transformPositions)
{
}

Tube::Tube(int num_cols, int num_rows) :
    Mesh(num_cols, num_rows)
{
}

std::vector<GLfloat> Tube::getPointData(int col, int row) {

    float x = 0.5 * cos( (static_cast<float>(col)/getNumCols()) * 2 * 3.1415 );
    float y = 0.5 * sin( (static_cast<float>(col)/getNumCols()) * 2 * 3.1415 );
    float z = (static_cast<float>(row)/getNumRows());

    glm::vec4 position = getTransformPositions() * glm::vec4(x, y, z, 1.0f);
    glm::vec4 normal = getTransformNormals() * glm::vec4(x, y, 0, 1.0f);

    return {position.x, position.y, position.z,
            normal.x, normal.y, normal.z};
}


Tube::~Tube()
{
}
