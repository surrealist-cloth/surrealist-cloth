#include "Cloth.h"

Cloth::~Cloth()
{
}

Cloth::Cloth(int rows, int cols) : m_rows(rows), m_cols(cols)
{
    // create masses
    m_masses.reserve(rows * cols);
    float x_step = m_width / m_cols;
    float y_step = m_height / m_rows;
    float x_init = -m_width / 2.0f;
    float y_init = -m_height / 2.0f;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            m_masses.push_back(std::make_unique<ClothMass>(glm::vec3(i * y_step - x_init, j * x_step - y_init, 0)));
        }
    }
    // connect the masses using constraints
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int index = i * cols + j;
            if (i < rows - 1)
            {
                m_constraints.push_back(
                    std::make_unique<ClothConstraint>(*m_masses[index], *m_masses[index + cols], m_stiffness));
            }
            if (j < cols - 1)
            {
                m_constraints.push_back(
                    std::make_unique<ClothConstraint>(*m_masses[index], *m_masses[index + 1], m_stiffness));
            }
        }
    }
}

void Cloth::step()
{
    for (int i = 0; i < m_constraintIterations; i++)
    {
        for (auto &constraint : m_constraints)
        {
            constraint->satisfyConstraint();
        }
    }
    for (auto &mass : m_masses)
    {
        mass->step(m_timeStep, m_damping);
    }
    avoidSelfCollisions();
}

void Cloth::translate(const glm::vec3 &translation)
{
    for (auto &mass : m_masses)
    {
        mass->translate(translation);
    }
}

void Cloth::addForce(const glm::vec3 &force)
{
    for (auto &mass : m_masses)
    {
        mass->addForce(force);
    }
}

void Cloth::addWindForce(const glm::vec3 &force)
{
    auto calculateWindForce = [](ClothMass &p1, ClothMass &p2, ClothMass &p3, const glm::vec3 &wind) -> glm::vec3 {
        glm::vec3 normal = glm::cross(p2.getPosition() - p1.getPosition(), p3.getPosition() - p1.getPosition());
        return normal * (glm::dot(glm::normalize(normal), wind));
    };
    for (int i = 0; i < m_rows - 1; i++)
    {
        for (int j = 0; j < m_cols - 1; j++)
        {
            int index = i * m_cols + j;
            glm::vec3 windForce1 =
                calculateWindForce(*m_masses[index], *m_masses[index + 1], *m_masses[index + m_cols], force);
            m_masses[index]->addForce(windForce1);
            m_masses[index + 1]->addForce(windForce1);
            m_masses[index + m_cols]->addForce(windForce1);
            glm::vec3 windForce2 = calculateWindForce(*m_masses[index], *m_masses[index + 1 + m_cols],
                                                      *m_masses[index + m_cols + 1], force);
            m_masses[index]->addForce(windForce2);
            m_masses[index + 1 + m_cols]->addForce(windForce2);
            m_masses[index + m_cols + 1]->addForce(windForce2);
        }
    }
}

void Cloth::avoidSelfCollisions()
{
    for (auto &mass : m_masses)
    {
        for (auto &other : m_masses)
        {
            if (mass != other)
            {
                glm::vec3 direction = mass->getPosition() - other->getPosition();
                float distance = glm::length(direction);
                if (distance < m_selfCollisionDistance)
                {
                    glm::vec3 correction = glm::normalize(direction) * (m_selfCollisionDistance - distance) / 2.0f;
                    mass->translate(correction);
                    other->translate(-correction);
                }
            }
        }
    }
}