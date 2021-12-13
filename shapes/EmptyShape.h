#ifndef EMTPYSHAPE_H
#define EMTPYSHAPE_H


#include "Shape.h"
#include "RectangleShape.h"
#include "PolygonShape.h"
#include <vector>

class EmptyShape : public Shape
{
public:
    EmptyShape();
    virtual ~EmptyShape() {};
    std::vector<Shape*> getChildren() override;
private:
    void generateShape() override;
};


#endif // EMTPYSHAPE_H
