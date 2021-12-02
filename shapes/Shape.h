#ifndef SHAPE_H
#define SHAPE_H

#include "Triangle.h"
#include <vector>
#include <glm/glm.hpp>

class Shape
{
public:
    Shape() {};
    virtual ~Shape() {};
    std::vector<Triangle> getTrianglesRecursive();
    std::vector<Triangle>& getTriangles();
    virtual std::vector<Shape*> getChildren() { return std::vector<Shape*>(); };
    void translate(glm::vec3 delta);
    void rotate(float angle, glm::vec3 axis);
    void scale(glm::vec3 scales);
    void transform(glm::mat4 transformation);
    void flushTransform();

protected:
    virtual void generateShape() = 0;

    std::vector<Triangle> m_triangles;
    glm::mat4 m_transformation = glm::mat4(1.0f);
};

#endif // SHAPE_H
