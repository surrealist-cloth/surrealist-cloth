#ifndef MESHISHAPE_H
#define MESHISHAPE_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "ishapes/IShape.h"
#include "scenegraph/RayScene.h"
#include "ishapes/TriangleIShape.h"

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
    MeshIShape(std::vector<Tri> triangles, std::vector<glm::vec3> vertices);
    MeshIShape(); // generate a default mesh
    //MeshIShape(string); //takes in string to a file path
    virtual ~MeshIShape() {};
    std::unique_ptr<glm::vec2> parameterize(glm::vec3& point) const override;
protected:
    std::vector<IntersectionCandidate> intersect(Ray& ray) const override;
private:
    std::vector<IntersectionCandidate> intersectTriangle(int triIndex, Ray &ray) const;

    void loadVertexTriangles();

    float getInterpolationWeight(int triIndex) const;
    glm::vec3 getVertexNormal(int vertexIndex) const;
    glm::vec3 getTriangleNormal(int triIndex) const;  // Returns unnormalized Normal!

    bool isWithinTriangle(int triIndex, glm::vec3 &point) const;
    glm::vec3 getNormal(int triIndex, glm::vec3 point) const;

    std::vector<Tri> m_triangles; //point to verts
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_vertexNormals; // might not even need this
    std::vector<std::vector<int>> m_vertexTriangles; // given a vertex, what triangles include it?

    //std::vector<glm::vec3> m_triangleNormals; //point to verts compute this in constructor
};

#endif // MESHISHAPE_H
