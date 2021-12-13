#include "ObjLoader.h"

#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "ishapes/MeshIShape.h"

ObjLoader::ObjLoader(std::string filePath)
{
    FILE * file = fopen(filePath.c_str(), "r");
    if( file == NULL ){
        std::cerr << "Impossible to open the file !" << std::endl;
        return;
    }

    while(true) {

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader
        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            m_vertices.push_back(vertex);
        } else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
        } else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            m_vertexNormals.push_back(normal);
        } else if ( strcmp( lineHeader, "f" ) == 0 ){
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return;
            }
            m_triangles.push_back(Tri(vertexIndex[0] - 1, vertexIndex[1] - 1, vertexIndex[2] - 1));
            m_normals.push_back(Tri(normalIndex[0] - 1, normalIndex[1] - 1, normalIndex[2] - 1));
        }
    }
}
