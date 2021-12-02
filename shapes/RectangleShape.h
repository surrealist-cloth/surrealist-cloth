#ifndef RECTANGLESHAPE_H
#define RECTANGLESHAPE_H

#include "Shape.h"

class RectangleShape : public Shape
{
public:
    RectangleShape(const int m, const int n);
    virtual ~RectangleShape() {};
private:
    void generateShape() override;
    const int m_m;
    const int m_n;
};

#endif // RECTANGLESHAPE_H
