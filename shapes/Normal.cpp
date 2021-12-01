#include "Normal.h"

Normal::Normal(int num_cols, int num_rows) :
    m_numCols(num_cols), m_numRows(num_rows)
{
}

Normal::~Normal()
{
}

std::vector<GLfloat> Normal::getNormal(int col, int row) {
    return {0, 0, 1};
}
