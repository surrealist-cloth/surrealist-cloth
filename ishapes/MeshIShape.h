#ifndef MESHISHAPE_H
#define MESHISHAPE_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "ishapes/IShape.h"
#include "scenegraph/RayScene.h"

// Points v_1 thru v_3 specified in counter-clockwise order
struct Tri {
    int v_1; // index of point a in triangle
    int v_2;
    int v_3;

    Tri() {}
    Tri(int v_1, int v_2, int v_3) : v_1(v_1), v_2(v_2), v_3(v_3) {}
};

class MeshIShape : public IShape
{
public:
    MeshIShape(std::string meshfile);
    MeshIShape(std::vector<glm::vec3> vertices, std::vector<Tri> triangles);
    virtual ~MeshIShape() {};
    std::unique_ptr<glm::vec2> parameterize(glm::vec3& point) const override;
protected:
    std::vector<IntersectionCandidate> intersect(const Ray& ray) const override;
private:
    inline std::unique_ptr<IntersectionCandidate> intersectTriangle(int index, const Ray &ray) const;

    void pruneInvalidTriangles();
    void loadVertexTriangles();
    void loadVertexNormals();

    inline glm::vec3 getTriangleNormal(const Tri tri) const;
    glm::vec3 getNormalBarycentric(int triIndex, glm::vec3 point) const;

    void loadDummyCloth();
    void loadTwoTriangles();
    void loadCube();

    std::vector<glm::vec3> m_vertices;
    std::vector<Tri> m_triangles; //point to verts

    std::vector<glm::vec3> m_vertexNormals; // might not even need this
    std::vector<std::vector<int>> m_vertexTriangles; // given a vertex, what triangles include it?
    std::vector<glm::vec3> m_triangleNormals; //point to verts compute this in constructor
};

#endif // MESHISHAPE_H
