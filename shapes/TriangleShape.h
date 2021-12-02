#ifndef TRIANGLESHAPE_H
#define TRIANGLESHAPE_H

#include "Shape.h"

class TriangleShape : public Shape
{
public:
    TriangleShape(const int n);
    virtual ~TriangleShape() {};
private:
    void generateShape() override;
    const int m_n;
};

#endif // TRIANGLESHAPE_H
