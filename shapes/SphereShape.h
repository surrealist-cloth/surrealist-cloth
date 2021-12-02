#ifndef SPHERESHAPE_H
#define SPHERESHAPE_H

#include "Shape.h"

class SphereShape : public Shape {
public:
    SphereShape(const int nSides, const int nSlices);
    virtual ~SphereShape() {};
private:
    void generateShape() override;
    inline int getIndex(int i, int j);
    const int m_nSides;
    const int m_nSlices;
};

#endif // SPHERESHAPE_H
