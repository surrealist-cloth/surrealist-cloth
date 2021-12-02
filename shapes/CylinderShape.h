#ifndef CYLINDERSHAPE_H
#define CYLINDERSHAPE_H

#include "Shape.h"
#include "RectangleShape.h"
#include "PolygonShape.h"
#include <vector>

class CylinderShape : public Shape
{
public:
    CylinderShape(const int nSides, const int nSlices);
    virtual ~CylinderShape() {};
    std::vector<Shape*> getChildren() override;
private:
    void generateShape() override;
    const int m_nSides;
    const int m_nSlices;
    std::unique_ptr<PolygonShape> m_top;
    std::unique_ptr<PolygonShape> m_bottom;
    std::vector<std::unique_ptr<RectangleShape>> m_sides;
};

#endif // CYLINDERSHAPE_H
