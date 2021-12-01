#include "Position.h"

Position::Position(int num_cols, int num_rows) :
    m_numCols(num_cols),
    m_numRows(num_rows)
{
}

std::vector<GLfloat> Position::getPosition(int col, int row){
    return {col/m_numCols, row/m_numRows, 0};
}

Position::~Position()
{
}
