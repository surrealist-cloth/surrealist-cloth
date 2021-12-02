#ifndef CONESHAPE_H
#define CONESHAPE_H

#include <vector>
#include <memory>
#include "Shape.h"
#include "PolygonShape.h"
#include "TriangleShape.h"

class ConeShape : public Shape {
public:
    ConeShape(const int nSides, const int nSlices);
    virtual ~ConeShape() {};
    std::vector<Shape*> getChildren() override;
private:
    void generateShape() override;
    const int m_nSides;
    const int m_nSlices;
    std::unique_ptr<PolygonShape> m_bottom;
    std::vector<std::unique_ptr<TriangleShape>> m_sides;
};

#endif // CONESHAPE_H
