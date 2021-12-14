#include "Cloth.h"
#include <fstream>
#include <iostream>
#include "ishapes/MeshIShape.h"

Cloth::~Cloth()
{
}

Cloth::Cloth(int rows, int cols) : m_rows(rows), m_cols(cols)
{
    // create masses
    m_masses.reserve(m_rows * m_cols);
    float x_step = m_width / m_cols;
    float z_step = m_height / m_rows;
    float x_init = -m_width / 2.0f;
    float z_init = m_height / 2.0f;
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            m_masses.push_back(std::make_unique<ClothMass>(glm::vec3(x_init + j * x_step, z_init - i * z_step, 0)));
            // m_masses.push_back(std::make_unique<ClothMass>(
            //     glm::vec3(m_width * (j / (float)m_cols), -m_height * (i / (float)m_rows), 0)));
        }
    }
    // connect the masses using constraints
    m_constraints.reserve(m_rows * m_cols);
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            int index = i * m_cols + j;
            // structural constraints
            if (i < m_rows - 1)
            {
                m_constraints.push_back(
                    std::make_unique<ClothConstraint>(*m_masses[index], *m_masses[index + m_cols], m_stiffness));
            }
            if (j < m_cols - 1)
            {
                m_constraints.push_back(
                    std::make_unique<ClothConstraint>(*m_masses[index], *m_masses[index + 1], m_stiffness));
            }
            // shear constraints
            if (i < m_rows - 1 && j < m_cols - 1)
            {
                m_constraints.push_back(
                    std::make_unique<ClothConstraint>(*m_masses[index], *m_masses[index + m_cols + 1], m_stiffness));
                m_constraints.push_back(
                    std::make_unique<ClothConstraint>(*m_masses[index + 1], *m_masses[index + m_cols], m_stiffness));
            }
            // bend constraints
            if (i < m_rows - 2)
            {
                m_constraints.push_back(
                    std::make_unique<ClothConstraint>(*m_masses[index], *m_masses[index + 2 * m_cols], m_stiffness));
            }
            if (j < m_cols - 2)
            {
                m_constraints.push_back(
                    std::make_unique<ClothConstraint>(*m_masses[index], *m_masses[index + 2], m_stiffness));
            }
            if (i < m_rows - 2 && j < m_cols - 2)
            {
                m_constraints.push_back(std::make_unique<ClothConstraint>(
                    *m_masses[index], *m_masses[index + 2 * m_cols + 2], m_stiffness));
                m_constraints.push_back(std::make_unique<ClothConstraint>(*m_masses[index + 2],
                                                                          *m_masses[index + 2 * m_cols], m_stiffness));
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
                calculateWindForce(*m_masses[index], *m_masses[index + m_cols], *m_masses[index + m_cols + 1], force);
            m_masses[index]->addForce(windForce1);
            m_masses[index + m_cols]->addForce(windForce1);
            m_masses[index + m_cols + 1]->addForce(windForce1);
            glm::vec3 windForce2 =
                calculateWindForce(*m_masses[index], *m_masses[index + m_cols + 1], *m_masses[index + 1], force);
            m_masses[index]->addForce(windForce2);
            m_masses[index + m_cols + 1]->addForce(windForce2);
            m_masses[index + 1]->addForce(windForce2);
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

void Cloth::toObj(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    std::cout << "Writing to " << filename << std::endl;
    std::vector<glm::vec3> vertices = getVertices();
    for (glm::vec3 &vertex: vertices)
    {
        file << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }

    std::vector<Tri> faces = getFaces();
    for (Tri& face: faces)
    {
        file << "f " << face.v_1 << " " << face.v_2 << " " << face.v_3 << std::endl;
    }
    file.close();
    std::cout << "Wrote " << vertices.size() << " vertices and " << faces.size() << " faces" << std::endl;
}

ClothMass &Cloth::massAt(int i, int j)
{
    return *m_masses[i * m_cols + j];
}

void Cloth::translateMass(ClothMass &mass, const glm::vec3 &translation)
{
    mass.translate(translation);
    for (auto &constraint : m_constraints)
    {
        if (&constraint->getMass1() == &mass || &constraint->getMass2() == &mass)
        {
            std::cout << "Updating constraint" << std::endl;
        }
    }
}

MeshIShape Cloth::toMesh() const
{
    return MeshIShape(getVertices(), getFaces());
}

std::vector<glm::vec3> Cloth::getVertices() const
{
    std::vector<glm::vec3> vertices;
    for (auto &mass : m_masses)
    {
        vertices.push_back(mass->getPosition());
    }
    return vertices;
}

std::vector<Tri> Cloth::getFaces() const
{
    std::vector<Tri> faces;
    for (int i = 0; i < m_rows - 1; i++)
    {
        for (int j = 0; j < m_cols - 1; j++)
        {
            int index = i * m_cols + j + 1;
            faces.push_back(Tri(index, index + m_cols, index + m_cols + 1));
            faces.push_back(Tri(index, index + m_cols + 1, index + 1));
//            faces.push_back(Tri(index, index + m_cols + 1, index + m_cols));
//            faces.push_back(Tri(index, index + 1, index + m_cols + 1));
        }
    }
    return faces;
}
