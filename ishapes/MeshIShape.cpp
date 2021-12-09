#include "MeshIShape.h"

MeshIShape::MeshIShape():
    m_triangles()
{
    m_triangles.reserve(1);

    glm::vec3 a = glm::vec3(-0.5f, -0.5f, 0.0f);
    glm::vec3 b = glm::vec3(0.5f, 0.5f, 0.0f);
    glm::vec3 c = glm::vec3(0.5f, 0.5f, 0.0f);
    TriangleIShape t = TriangleIShape(a,b,c);

    m_triangles.push_back(t);
}



std::vector<float> MeshIShape::intersect(Ray &ray) const {
    std::vector<float> ts;

    for (int i = 0; i < m_triangles.size(); i++) { //TO IMPROVE: I'm copying all the triangles
        for (float i : m_triangles[i].allIntersect(ray) ) {
             ts.push_back(i);
        }
    }
    return ts;
}

std::unique_ptr<glm::vec3> MeshIShape::getNormal(glm::vec3 &point) const {
    return std::make_unique<glm::vec3>(glm::vec3(0.0f, 0.0f, 1.0f));
}

std::unique_ptr<glm::vec2> MeshIShape::parameterize(glm::vec3 &point) const {
    return std::make_unique<glm::vec2>(0,0);
}
