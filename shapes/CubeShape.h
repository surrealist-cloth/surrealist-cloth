#ifndef CUBESHAPE_H
#define CUBESHAPE_H

#include "Shape.h"
#include <memory>
#include <vector>
#include "RectangleShape.h"

class CubeShape : public Shape
{
public:
    CubeShape(const int n);
    virtual ~CubeShape() {};
    std::vector<Shape*> getChildren() override;
private:
    void generateShape() override;
    const int m_n;
    std::unique_ptr<RectangleShape> m_top;
    std::unique_ptr<RectangleShape> m_left;
    std::unique_ptr<RectangleShape> m_front;
    std::unique_ptr<RectangleShape> m_right;
    std::unique_ptr<RectangleShape> m_back;
    std::unique_ptr<RectangleShape> m_bottom;
};

#endif // CUBESHAPE_H
