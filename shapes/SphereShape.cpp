#include "SphereShape.h"
#include "Triangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <vector>

SphereShape::SphereShape(const int nSides, const int nSlices) : m_nSides(nSides), m_nSlices(nSlices) {
    if (nSides < 3) {
        throw std::runtime_error("nSides must be at least 3");
    }
    if (nSlices < 2) {
        throw std::runtime_error("nSlices must be at least 2");
    }
    SphereShape::generateShape();
}

inline glm::vec3 sphericalToCartesian(float theta, float phi, float radius=0.5f) {
    return glm::vec3(glm::sin(phi) * glm::cos(theta) * radius, glm::cos(phi) * radius, glm::sin(phi) * glm::sin(theta) * radius);
}

int SphereShape::getIndex(int i, int j) {
    int displacement = (j == 0) ? 0 : 2 * j - 1;
    return i * (2 * m_nSlices - 2) + displacement;
}


void SphereShape::generateShape() {
    const float unitTheta = glm::pi<float>() * 2.f / static_cast<float>(m_nSides);
    const float unitPhi = glm::pi<float>() / static_cast<float>(m_nSlices);
    for (int i = 0; i < m_nSides; i++) {
        for (int j = 0; j < m_nSlices; j++) {
            glm::vec3 c = sphericalToCartesian(static_cast<float>(i) * unitTheta, static_cast<float>(j) * unitPhi);
            glm::vec3 d = sphericalToCartesian(static_cast<float>(i + 1) * unitTheta, static_cast<float>(j) * unitPhi);
            glm::vec3 a = sphericalToCartesian(static_cast<float>(i + 1) * unitTheta, static_cast<float>(j + 1) * unitPhi);
            glm::vec3 b = sphericalToCartesian(static_cast<float>(i) * unitTheta, static_cast<float>(j + 1) * unitPhi);
            if (j != (m_nSlices - 1)) m_triangles.push_back(Triangle(a, b, c));
            if (j != 0) m_triangles.push_back(Triangle(c, d, a));
        }
    }

    // average the normals between sides
    for (int i = 0; i < m_nSides; i++) {
        int neighborI = (i + 1) % m_nSides;
        for (int j = 0; j < (m_nSlices - 1); j++) {
            int neighborJ = j + 1;
            Normal normal = Triangle::averageNormals({ m_triangles[getIndex(i, j)].defaultNormal(),
                                                       m_triangles[getIndex(neighborI, j)].defaultNormal(),
                                                       m_triangles[getIndex(i, neighborJ)].defaultNormal(),
                                                       m_triangles[getIndex(neighborI, neighborJ)].defaultNormal()
                                                     });
            m_triangles[getIndex(i, j)].overrideANormal(normal);
            if (j != 0) m_triangles[getIndex(i, j) + 1].overrideCNormal(normal);

            m_triangles[getIndex(neighborI, j)].overrideBNormal(normal);
            if (neighborJ == (m_nSlices - 1)) {
                m_triangles[getIndex(i, neighborJ)].overrideBNormal(normal);
                m_triangles[getIndex(neighborI, neighborJ)].overrideANormal(normal);
            } else {
                m_triangles[getIndex(i, neighborJ) + 1].overrideBNormal(normal);
                m_triangles[getIndex(neighborI, neighborJ)].overrideCNormal(normal);
                m_triangles[getIndex(neighborI, neighborJ) + 1].overrideANormal(normal);
            }
        }

        // do the two ends
        m_triangles[getIndex(i, 0)].overrideCNormal(glm::vec3(0.0f, 1.0f, 0.0f));
        m_triangles[getIndex(i, (m_nSlices - 1))].overrideCNormal(glm::vec3(0.0f, -1.0f, 0.0f));
    }
}


