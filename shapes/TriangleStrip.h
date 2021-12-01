#ifndef TriangleStrip_H
#define TriangleStrip_H

#include "GL/glew.h"
#include <vector>
#include<memory>

#include "Position.h"
#include "Normal.h"
#include "Mesh.h"

/**
 * @brief TriangleStrip::TriangleStrip
 * Contains methods to create TriangleStripes of triangle strips,
 * and move them.
 */
class TriangleStrip
{
public:
    TriangleStrip();

    std::vector<GLfloat> buildTriangleStrip(int numCols, int numRows, Mesh &verticies);
    std::vector<GLfloat> buildTriangleStrip(int numRows, int numCols, Position p, Normal n);

    //adds a TriangleStrip to the end of a vector
    void addTriangleStripTo(std::vector<GLfloat> *vector,
                   int numCols, int numRows, Mesh &verticies);
    void translate(
            float x, float y, float z, std::vector<GLfloat>* TriangleStrip);
    void rotate(float angle,
            float x, float y, float z, std::vector<GLfloat>* TriangleStrip);

    void transform(glm::mat4 mat, std::vector<GLfloat>* TriangleStrip);

    ~TriangleStrip();
private:
    void print(std::vector<GLfloat> data);
};

#endif // TriangleStrip_H
