#ifndef NORMAL_H
#define NORMAL_H

#include "GL/glew.h"

#include<memory>
#include <vector>
/**
 * @brief The Normal class
 * Returns a vector of three GLfloats
 * describing a normal vector.
 *
 * By default, this class returns normals pointing
 * in the z direction.
 */
class Normal
{
public:
    Normal();
    Normal(int num_cols, int num_rows);
    virtual ~Normal();
    std::vector<GLfloat> getNormal(int col, int row);
private:
    const float m_numCols, m_numRows;

};

#endif // NORMAL_H
