#include "Cloth.h"

Cloth::Cloth(int rows, int cols) : m_rows(rows), m_cols(cols)
{
    // create masses
    m_masses.reserve(rows * cols);
    float x_step = m_width / m_cols;
    float y_step = m_height / m_rows;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            m_masses.push_back(std::make_unique<ClothMass>(glm::vec3(i * y_step, j * x_step, 0)));
        }
    }
    // connect the masses using constraints
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int index = i * cols + j;
            if (i > 0)
            {
                m_constraints.push_back(
                    std::make_unique<ClothConstraint>(m_masses[index], m_masses[index - cols], m_stiffness));
            }
            if (i < rows - 1)
            {
                m_constraints.push_back(
                    std::make_unique<ClothConstraint>(m_masses[index], m_masses[index + cols], m_stiffness));
            }
            if (j > 0)
            {
                m_constraints.push_back(
                    std::make_unique<ClothConstraint>(m_masses[index], m_masses[index - 1], m_stiffness));
            }
            if (j < cols - 1)
            {
                m_constraints.push_back(
                    std::make_unique<ClothConstraint>(m_masses[index], m_masses[index + 1], m_stiffness));
            }
        }
    }
}
