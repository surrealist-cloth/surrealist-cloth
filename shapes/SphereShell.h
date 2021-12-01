#ifndef SPHERESHELL_H
#define SPHERESHELL_H

#include "GL/glew.h"
#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"

#include<memory>
#include <vector>

#include "Mesh.h"

/**
 * @brief The SphereShell class
 * Given a column and row, returns the data for a specific point on a plane
 * as a vector of GLfloats. This data includes the x, y, z coordinates
 * describing the position of a point, followed by the x, y, z coordinates
 * of the normal at that point.
 *
 * If a transformPositions is not given for getPointData(col, row, transformPositions),
 * then by default the plane is on the XY plane from 0 to 1 in dimensions x and y.
 */
class SphereShell : public Mesh
{
public:

    SphereShell(int num_cols, int num_rows,
                             glm::mat4 transformPositions, glm::mat4 transformNormals);
    SphereShell(int numCols, int numRows, glm::mat4 transformPositions);
    SphereShell(int numCols, int numRows);

    std::vector<GLfloat> getPointData(int col, int row) override;

    ~SphereShell();
};

#endif // SPHERESHELL_H
