#ifndef Position_H
#define Position_H

#include "GL/glew.h"

#include<memory>
#include <vector>

/**
 * @brief The Position class
 * This class returns a vector of three GLfloats
 * describing a position in 3d space.
 *
 * By default, getPosition() returns points on the
 * y, x plane between [0,1] for x and y.
 */

class Position
{
public:
    Position(int num_cols, int num_rows);
    virtual ~Position();
    std::vector<GLfloat> getPosition(int col, int row);
private:
    const float m_numCols, m_numRows;
};

#endif // Position_H
