#ifndef POLYGONSHAPE_H
#define POLYGONSHAPE_H

#include "Shape.h"
#include "TriangleShape.h"
#include <vector>
#include <memory>

class PolygonShape : public Shape
{
public:
    PolygonShape(const int nSides, const int nSlices);
    virtual ~PolygonShape() {};
    std::vector<Shape*> getChildren() override;
private:
    void generateShape() override;
    const int m_nSides;
    const int m_nSlices;
    std::vector<std::unique_ptr<TriangleShape>> m_slices;
};

#endif // POLYGONSHAPE_H
