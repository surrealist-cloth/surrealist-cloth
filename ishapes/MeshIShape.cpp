#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

//#include <Qt3DCore>
//#include <Qt3DInput>

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

MeshIShape::MeshIShape(std::string meshfile) {
    if (meshfile == "cube") {
        loadCube();
    } else if (meshfile == "triangles") {
        loadTwoTriangles();
    } else {
        loadDummyCloth();
    }
    pruneInvalidTriangles();
    loadVertexTriangles();
    loadVertexNormals();
}

MeshIShape::MeshIShape(std::vector<glm::vec3> vertices, std::vector<Tri> triangles):
    m_vertices(vertices), m_triangles(triangles)
{   
    pruneInvalidTriangles();
    loadVertexTriangles();
    loadVertexNormals();
}



std::vector<IntersectionCandidate> MeshIShape::intersect(Ray &ray) const {
    std::vector<IntersectionCandidate> ts;

    for (int i = 0; i < m_triangles.size(); i++) {
        std::unique_ptr<IntersectionCandidate> t = intersectTriangle(i, ray);
        if (t) {
            ts.push_back(*t);
        }
    }
    return ts;
}



std::unique_ptr<IntersectionCandidate> MeshIShape::intersectTriangle(int index, const Ray &ray) const {
    const Tri& tri = m_triangles[index];
    const glm::vec3 v_1 = m_vertices[tri.v_1];
    const glm::vec3 v_2 = m_vertices[tri.v_2];
    const glm::vec3 v_3 = m_vertices[tri.v_3];
    const glm::vec3 n = m_triangleNormals[index];


    float d = glm::dot(n, v_1);
    float denom = glm::dot(n, ray.dir);
    // denom is 0 if ray is parallel to triangle
    if (denom == 0.f) return std::unique_ptr<IntersectionCandidate>(nullptr);

    float t = -(glm::dot(m_triangleNormals[index], ray.eye) + d) / denom;
    glm::vec3 contact = ray.getPoint(t);

    // check if ray intersection with plane is inside triangle
    // (if on inside line of all edges by checking normal's direction)
    if (glm::dot(n, glm::cross(v_2 - v_1, contact - v_1)) < 0 ||
            glm::dot(n, glm::cross(v_3 - v_2, contact - v_2)) < 0 ||
            glm::dot(n, glm::cross(v_1 - v_3, contact - v_3)) < 0) {
        return std::unique_ptr<IntersectionCandidate>(nullptr);
    }

    // remove back facing triangles
    if (glm::dot(n, ray.dir) > 0.f) {
        return std::unique_ptr<IntersectionCandidate>(nullptr)
;    }


    return std::make_unique<IntersectionCandidate>(t,
                                                   [&](glm::vec3 point){ return getNormalBarycentric(index, point);});
}

void MeshIShape::pruneInvalidTriangles()
{
    std::vector<Tri> triangles;
    triangles.reserve(m_triangles.size());
    for (Tri tri : m_triangles) {
        if (m_vertices.size() < tri.v_1 || m_vertices.size() < tri.v_2
              || m_vertices.size() < tri.v_3) {
            std::cerr << "Invalid triangle. Skipping" << std::endl;
            continue;
        }
        triangles.push_back(tri);
    }
    m_triangles = triangles;
}

glm::vec3 MeshIShape::getNormalBarycentric(int triIndex, glm::vec3 point) const {
    return m_triangleNormals[triIndex];

    const Tri tri = m_triangles[triIndex];
    const glm::vec3 n_1 = m_vertexNormals[tri.v_1];
    const glm::vec3 n_2 = m_vertexNormals[tri.v_2];
    const glm::vec3 n_3 = m_vertexNormals[tri.v_3];
    const glm::vec3 v_1 = m_vertices[tri.v_1];
    const glm::vec3 v_2 = m_vertices[tri.v_2];
    const glm::vec3 v_3 = m_vertices[tri.v_3];

    float triangleArea2 = glm::length(glm::cross(v_2 - v_1, v_3 - v_2));

    float subtriangleArea2 = glm::length(glm::cross(point - v_1, v_3 - point));
    float u = subtriangleArea2 / triangleArea2;

    subtriangleArea2 = glm::length(glm::cross(v_2 - v_1, point - v_2));
    float v = subtriangleArea2 / triangleArea2;

    float w = 1.f - u - v;

    return glm::normalize(u * n_2 + v * n_3 + w * n_1);
}

std::unique_ptr<glm::vec2> MeshIShape::parameterize(glm::vec3 &point) const {
    return std::make_unique<glm::vec2>(0,0);
}

void MeshIShape::loadVertexTriangles() {
    m_vertexTriangles.clear();
    m_vertexTriangles.resize(m_vertices.size());
    for (int triIndex = 0; triIndex < m_triangles.size(); triIndex++) {
        const Tri tri = m_triangles[triIndex];
        m_vertexTriangles[tri.v_1].push_back(triIndex);
        m_vertexTriangles[tri.v_2].push_back(triIndex);
        m_vertexTriangles[tri.v_3].push_back(triIndex);
    }
}

void MeshIShape::loadVertexNormals() {
    m_triangleNormals.reserve(m_triangles.size());

    for (const Tri tri: m_triangles) {
        m_triangleNormals.push_back(getTriangleNormal(tri));
    }

    m_vertexNormals.clear();
    m_vertexNormals.reserve(m_vertices.size());
    for (int i = 0; i < m_vertices.size(); i++) {
        glm::vec3 normal(0);
        for (const int triIndex : m_vertexTriangles[i]) {
            normal += m_triangleNormals[triIndex];
        }
        m_vertexNormals.push_back(glm::normalize(normal));
    }

    for (int i = 0; i < m_triangleNormals.size(); i++) {
        m_triangleNormals[i] = glm::normalize(m_triangleNormals[i]);
    }
}

glm::vec3 MeshIShape::getTriangleNormal(const Tri tri) const { //Returns unnormalized Normal!
    return glm::cross(m_vertices[tri.v_2] - m_vertices[tri.v_1],
            m_vertices[tri.v_3] - m_vertices[tri.v_1]);
}

void MeshIShape::loadCube() {
    std::vector<glm::vec3> vertices;
    std::vector<Tri> triangles;

    vertices.resize(8);
    triangles.resize(12);

    vertices[0] = glm::vec3(-0.5f, 0.5f , 0.5f);
    vertices[1] = glm::vec3(-0.5f, 0.5f ,-0.5f);
    vertices[2] = glm::vec3(0.5f , 0.5f ,-0.5f);
    vertices[3] = glm::vec3(0.5f , 0.5f , 0.5f);
    vertices[4] = glm::vec3(-0.5f ,-0.5f ,0.5f);
    vertices[5] = glm::vec3(-0.5f ,-0.5f ,-0.5f);
    vertices[6] = glm::vec3(0.5f ,-0.5f ,-0.5f);
    vertices[7] = glm::vec3(0.5f ,-0.5f ,0.5f);

    triangles[0] = Tri(0, 2, 1);
    triangles[1] = Tri(0, 3, 2);
    triangles[2] = Tri(3, 6, 2);
    triangles[3] = Tri(3, 7, 6);
    triangles[4] = Tri(4, 7, 0);
    triangles[5] = Tri(0, 7, 3);
    triangles[6] = Tri(4, 6, 7);
    triangles[7] = Tri(4, 5, 6);
    triangles[8] = Tri(0, 5, 4);
    triangles[9] = Tri(0, 1, 5);
    triangles[10] = Tri(1, 6, 5);
    triangles[11] = Tri(1, 2, 6);

    m_triangles = triangles;
    m_vertices = vertices;
}

void MeshIShape::loadTwoTriangles() {
    std::vector<glm::vec3> vertices;
    std::vector<Tri> triangles;

    vertices.resize(5);
    triangles.resize(2);

    vertices[0] = glm::vec3(-0.25f, 0.5f , 0.5f);
    vertices[1] = glm::vec3( 0.25f, 0.4f , 0.0f);
    vertices[2] = glm::vec3(-0.5f , 0.0f , 0.0f);
    vertices[3] = glm::vec3( 0.0f , 0.0f , 0.0f);
    vertices[4] = glm::vec3( 0.5f , 0.0f , -0.5f);

    triangles[0] = Tri(2, 3, 0);
    triangles[1] = Tri(3, 4, 1);

    m_vertices = vertices;
    m_triangles = triangles;
}

void MeshIShape::loadDummyCloth() {
    std::vector<Tri> triangles;
    std::vector<glm::vec3> vertices;
    triangles.resize(8);
    vertices.resize(9);

    triangles[0] = Tri(3, 4, 0);
//    triangles[1] = Tri(0, 4, 1);
//    triangles[2] = Tri(4, 2, 1);
//    triangles[3] = Tri(4, 5, 2);
    triangles[4] = Tri(4, 8, 5);
    triangles[5] = Tri(7, 8, 4);
//    triangles[6] = Tri(6, 7, 4);
//    triangles[7] = Tri(6, 4, 3);

    vertices[0] = glm::vec3(-0.5f, 0.5f, 0.0f);
    vertices[1] = glm::vec3( 0.0f, 0.5f, 0.0f);
    vertices[2] = glm::vec3( 0.5f, 0.5f, 0.0f);

    vertices[3] = glm::vec3(-0.5f, 0.0f, 0.0f);
    vertices[4] = glm::vec3( 0.0f, 0.0f, 0.0f);
    vertices[5] = glm::vec3( 0.5f, 0.0f, -0.5f);

    vertices[6] = glm::vec3(-0.5f,-0.5f, 0.0f);
    vertices[7] = glm::vec3( 0.0f,-0.5f, 0.0f);
    vertices[8] = glm::vec3( 0.5f,-0.5f, 0.0f);

    m_triangles = triangles;
    m_vertices = vertices;
}
