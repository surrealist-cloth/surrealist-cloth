#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include "ishapes/MeshIShape.h"
#include "ishapes/IShape.h"
#include "ishapes/TriangleIShape.h"

//Do it with pointers
//Triangles with collisions

//Generate grid of points

//Vertex objects make a wrapper-> update position update velocity, have index, what is pointer to triangle?

//Iterate through grid of points
//The triangle object contain pointers to vertex object


//could have inverse point point to each other.

MeshIShape::MeshIShape() {
    std::vector<Tri> triangles;
    std::vector<glm::vec3> vertices;
    triangles.resize(1);
    vertices.resize(3);

    triangles[0] = Tri(0, 1, 2);

    vertices[0] = glm::vec3(-0.5f, -0.5f, 0.0f);
    vertices[1] = glm::vec3(0.5f, -0.5f, 0.0f);
    vertices[2] = glm::vec3(0.0f, 0.5f, 0.0f);

    m_vertexNormals.resize(vertices.size());
    m_vertexTriangles.resize(vertices.size());

    m_triangles = triangles;
    m_vertices = vertices;
    loadVertexTriangles();
    assert(m_vertices.size() == m_vertexTriangles.size());
}

MeshIShape::MeshIShape(std::vector<Tri> triangles, std::vector<glm::vec3> vertices):
    m_triangles(triangles), m_vertices(vertices)
{   
    //loadTriangles();
    m_vertexNormals.resize(vertices.size());
    m_vertexTriangles.resize(vertices.size());
    loadVertexTriangles();

    assert(m_vertices.size() == m_vertexTriangles.size());
}



std::vector<IntersectionCandidate> MeshIShape::intersect(Ray &ray) const {
    std::vector<IntersectionCandidate> ts;

    for (int i = 0; i < m_triangles.size(); i++) { //TO IMPROVE: I'm copying all the triangles

        for (IntersectionCandidate &i : intersectTriangle(i, ray) ) {
             ts.push_back(i);
        }
    }
    return ts;
}



std::vector<IntersectionCandidate> MeshIShape::intersectTriangle(int triIndex, Ray &ray) const {
    std::vector<IntersectionCandidate> ts;
    float t;

    glm::vec3 v_1 = m_vertices[m_triangles[triIndex].v_1];
    glm::vec3 v_2 = m_vertices[m_triangles[triIndex].v_2];
    glm::vec3 v_3 = m_vertices[m_triangles[triIndex].v_3];
    glm::vec3 n = glm::normalize(getTriangleNormal(triIndex));

    if (v_1 == v_2 || v_2 == v_3 || v_3 == v_1) { //check if Tri has surface
        return ts;
    }

    float d = glm::dot(n, v_1);

    // find point p + dt on the triangle's plane
    if (glm::epsilonEqual(glm::dot(n, ray.dir), 0.0f, EPSILON)) { // check d not parallel to plane
        return ts;
    }
    t = (d - glm::dot(n, ray.eye)) / (glm::dot(n, ray.dir));

    // check if ray intersection with plane is inside triangle
    // (if on inside line of all edges by checking normal's direction)
    if (glm::dot(glm::cross(v_2 - v_1, (ray.eye + t * ray.dir) - v_1), n) < 0) {
        return ts;
    }
    if (glm::dot(glm::cross(v_3 - v_2, (ray.eye + t * ray.dir) - v_2), n) < 0) {
        return ts;
    }
    if (glm::dot(glm::cross(v_1 - v_3, (ray.eye + t * ray.dir) - v_3), n) < 0) {
        return ts;
    }

    ts.push_back(IntersectionCandidate(t, [&](glm::vec3 point){
                                                            return n;
                                      })
                );
    return ts;
}

glm::vec3 MeshIShape::getNormal(int triIndex, glm::vec3 point) const {
    if (!isWithinTriangle(triIndex, point)) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::vec3 n_1 = getVertexNormal(m_triangles[triIndex].v_1);
    glm::vec3 n_2 = getVertexNormal(m_triangles[triIndex].v_2);
    glm::vec3 n_3 = getVertexNormal(m_triangles[triIndex].v_3);
    //TODO: compute weighted average of these points
    //      by converting from Cartesian coords to barycentric

    return getTriangleNormal(triIndex);
}

bool MeshIShape::isWithinTriangle(int triIndex, glm::vec3 &point) const {
    glm::vec3 v_1 = m_vertices[m_triangles[triIndex].v_1];
    glm::vec3 v_2 = m_vertices[m_triangles[triIndex].v_2];
    glm::vec3 v_3 = m_vertices[m_triangles[triIndex].v_3];
    glm::vec3 n = glm::normalize(getTriangleNormal(triIndex));

    if (glm::dot(glm::cross(v_2 - v_1, (point) - v_1), n) < 0) {
        return false;
    }
    if (glm::dot(glm::cross(v_3 - v_2, (point) - v_2), n) < 0) {
        return false;
    }
    if (glm::dot(glm::cross(v_1 - v_3, (point) - v_3), n) < 0) {
        return false;
    }
    return true;
}

std::unique_ptr<glm::vec2> MeshIShape::parameterize(glm::vec3 &point) const {
    return std::make_unique<glm::vec2>(0,0);
}

void MeshIShape::loadVertexTriangles() {
    for (int triIndex; triIndex < m_triangles.size(); triIndex++) {
        Tri tri = m_triangles[triIndex];
        m_vertexTriangles[tri.v_1].push_back(triIndex);
        m_vertexTriangles[tri.v_2].push_back(triIndex);
        m_vertexTriangles[tri.v_3].push_back(triIndex);
    }
}


glm::vec3 MeshIShape::getVertexNormal(int vertexIndex) const {
    if (vertexIndex < 0 || vertexIndex >= m_vertexTriangles.size()) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::vec3 n_v = glm::vec3(0.0f, 0.0f, 0.0f);

    for (int neighbourTriIndex : m_vertexTriangles[vertexIndex]) {
        glm::vec3 neighbourTriNormal = getTriangleNormal(neighbourTriIndex);
         n_v = n_v + getInterpolationWeight(neighbourTriIndex) * neighbourTriNormal;
    }
    return n_v;
}

float MeshIShape::getInterpolationWeight(int triIndex) const {
    if (triIndex < 0 || triIndex >= m_triangles.size()) {
        return 0.0f;
    }

    glm::vec3 normal = getTriangleNormal(triIndex); // not normalized
    float normalLength = sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));
    return 0.5f * normalLength;
}

glm::vec3 MeshIShape::getTriangleNormal(int triIndex) const { //Returns unnormalized Normal!
    if (triIndex < 0 || triIndex >= m_triangles.size()) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
    glm::vec3 dist1 = m_vertices[m_triangles[triIndex].v_2]
                                    - m_vertices[m_triangles[triIndex].v_1];
    glm::vec3 dist2 = m_vertices[m_triangles[triIndex].v_3]
                                    - m_vertices[m_triangles[triIndex].v_1];
    return glm::cross(dist1, dist2);
}

