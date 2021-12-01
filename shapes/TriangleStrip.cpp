#include <iostream>
#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"

#include "TriangleStrip.h"
#include "Position.h"
#include "Normal.h"

#include "Constants.h"


TriangleStrip::TriangleStrip()
{
}

std::vector<GLfloat> TriangleStrip::buildTriangleStrip(int numCols, int numRows, Mesh &verticies) {

    //Data will hold the triangle strips (there are 2 triangles per index making 6 points.)
    std::vector<GLfloat> data(numRows * numCols * 6 * ARRAYS_PER_POINT * FLOATS_PER_ARRAY);
    addTriangleStripTo(&data, numCols, numRows, verticies);
    return data;
}


void TriangleStrip::addTriangleStripTo(std::vector<GLfloat> *vector,
                         int numCols, int numRows, Mesh &verticies) {

    numCols ++;
    numRows ++;
    for (int row = 0; row < numRows - 1; row++) {

        for (int col = 0; col < numCols - 1; col++) {

            std::vector<GLfloat> v1 =  verticies.getPointData(col, row);
            std::vector<GLfloat> v2 =  verticies.getPointData(col + 1, row);
            std::vector<GLfloat> v3 =  verticies.getPointData(col + 1, row + 1);
            std::vector<GLfloat> v4 =  verticies.getPointData(col, row + 1);

            //lower triangle:

            vector->insert(vector->end(), v1.begin(), v1.end());
            vector->insert(vector->end(), v2.begin(), v2.end());
            vector->insert(vector->end(), v3.begin(), v3.end());

            //higher triangle:

            vector->insert(vector->end(), v1.begin(), v1.end());
            vector->insert(vector->end(), v3.begin(), v3.end());
            vector->insert(vector->end(), v4.begin(), v4.end());
        }
    }

}


void TriangleStrip::transform(glm::mat4 mat, std::vector<GLfloat>* TriangleStrip) {

    for (unsigned long pointData = 0; pointData < TriangleStrip->size();
          pointData = pointData + FLOATS_PER_ARRAY) {

        if (pointData % 2 != 1) {
        glm::vec4 point =
                glm::vec4( (*TriangleStrip)[pointData], (*TriangleStrip)[pointData + 1],
                (*TriangleStrip)[pointData + 2], 1.0f);


        glm::vec4 transformedVector = mat * point;

        (*TriangleStrip)[pointData] = transformedVector.x;
        (*TriangleStrip)[pointData + 1] = transformedVector.y;
        (*TriangleStrip)[pointData + 2] = transformedVector.z;

        }
    }
}


void TriangleStrip::translate(
        float x, float y, float z, std::vector<GLfloat>* TriangleStrip) {

    for (unsigned long pointData = 0; pointData < TriangleStrip->size();
          pointData = pointData + FLOATS_PER_ARRAY) {

        if (pointData % 2 != 1) {
        glm::vec4 point =
                glm::vec4( (*TriangleStrip)[pointData], (*TriangleStrip)[pointData + 1],
                (*TriangleStrip)[pointData + 2], 1.0f);

        glm::mat4 translateMatrix = glm::translate(glm::vec3(x, y, z));

        glm::vec4 transformedVector = translateMatrix * point;

        (*TriangleStrip)[pointData] = transformedVector.x;
        (*TriangleStrip)[pointData + 1] = transformedVector.y;
        (*TriangleStrip)[pointData + 2] = transformedVector.z;
        }
    }
}

void TriangleStrip::rotate(float angle,
        float x, float y, float z, std::vector<GLfloat>* TriangleStrip) {
    for (unsigned long pointData = 0; pointData < TriangleStrip->size();
          pointData = pointData + FLOATS_PER_ARRAY) {

        glm::vec4 pointOrNormal =
                glm::vec4( (*TriangleStrip)[pointData], (*TriangleStrip)[pointData + 1],
                (*TriangleStrip)[pointData + 2], 1.0f);

        glm::mat4 rotateMatrix = glm::rotate(angle, glm::vec3(x, y, z));

        glm::vec4 transformedVector = rotateMatrix * pointOrNormal;

        (*TriangleStrip)[pointData] = transformedVector.x;
        (*TriangleStrip)[pointData + 1] = transformedVector.y;
        (*TriangleStrip)[pointData + 2] = transformedVector.z;
    }

}


void TriangleStrip::print(std::vector<GLfloat> data) {

    std::cout << "Printing data of size " << data.size() << std::endl;

    for (unsigned long i = 0; i < data.size() / 3; i++ ) {
        for (int j = 0; j < 3; j++) {
            std::cout << data[i*3 + j] << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

TriangleStrip::~TriangleStrip(){
}
