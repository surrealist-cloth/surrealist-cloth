#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <ishapes/MeshIShape.h>

class ObjLoader
{
public:
    ObjLoader(std::string filePath);
    std::vector<glm::vec3> m_vertices;
    std::vector<Tri> m_triangles;
    std::vector<Tri> m_normals;
    std::vector<glm::vec3> m_vertexNormals;
};

#endif // OBJLOADER_H
