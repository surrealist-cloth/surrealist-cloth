#include "ObjLoader.h"

#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <fstream>
#include <regex>
#include "ishapes/MeshIShape.h"

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

ObjLoader::ObjLoader(std::string filePath)
{
    std::ifstream file(filePath);
    if (file.fail()) {
        std::cerr << "Cannot open file" << std::endl;
        return;
    }

    for (std::string line; std::getline(file, line); ) {
        int splitIndex = line.find(" ");
        std::string lineHeader = line.substr(0, splitIndex);
        if (splitIndex == std::string::npos) continue;
        std::vector<std::string> parts = split(line.substr(splitIndex+1), " ");
        if (parts.size() != 3) {
            std::cerr << "Unparseable file format" << std::endl;
            return;
        }
        if (lineHeader == "v") {
            glm::vec3 vertex(std::stof(parts[0]), std::stof(parts[1]), std::stof(parts[2]));
            m_vertices.push_back(vertex);
        } else if (lineHeader == "vn") {
            glm::vec3 normal(std::stof(parts[0]), std::stof(parts[1]), std::stof(parts[2]));
            m_vertexNormals.push_back(normal);
        } else if (lineHeader == "f") {
            std::vector<std::string> parts1 = split(parts[0], "/");
            std::vector<std::string> parts2 = split(parts[1], "/");
            std::vector<std::string> parts3 = split(parts[2], "/");

            if (parts1.size() == 1) {
                if (parts2.size() != 1 || parts3.size() != 1) {
                    std::cerr << "Unparseable file format" << std::endl;
                    return;
                }
                m_triangles.push_back(Tri(std::stoi(parts1[0]) - 1, std::stoi(parts2[0]) - 1, std::stoi(parts3[0]) - 1));
                hasNormals = false;
                continue;
            }

            if (parts1.size() == 2) {
                if (parts2.size() != 2 || parts3.size() != 2) {
                    std::cerr << "Unparseable file format" << std::endl;
                    return;
                }
                m_triangles.push_back(Tri(std::stoi(parts1[0]) - 1, std::stoi(parts2[0]) - 1, std::stoi(parts3[0]) - 1));
                m_normals.push_back(Tri(std::stoi(parts1[1]) - 1, std::stoi(parts2[1]) - 1, std::stoi(parts3[1]) - 1));
                hasNormals = true;
                continue;
            }

            if (parts1.size() == 3) {
                if (parts2.size() != 3 || parts3.size() != 3) {
                    std::cerr << "Unparseable file format" << std::endl;
                    return;
                }
                m_triangles.push_back(Tri(std::stoi(parts1[0]) - 1, std::stoi(parts2[0]) - 1, std::stoi(parts3[0]) - 1));
                m_normals.push_back(Tri(std::stoi(parts1[2]) - 1, std::stoi(parts2[2]) - 1, std::stoi(parts3[2]) - 1));
                hasNormals = true;
                continue;
            }

            std::cerr << "Unparseable file format" << std::endl;
            return;
        }
    }
}
