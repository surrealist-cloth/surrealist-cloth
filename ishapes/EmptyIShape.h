#ifndef EMPTYISHAPE_H
#define EMPTYISHAPE_H

#include "IShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "scenegraph/RayScene.h"

class EmptyIShape : public IShape
{
public:
    static EmptyIShape &shared_instance() {static EmptyIShape shape; return shape;}
    EmptyIShape();
    virtual ~EmptyIShape() {};
protected:
    std::vector<IntersectionCandidate> intersect(Ray& ray) const override;
};

EmptyIShape emptyIShape();

#endif // EMPTYISHAPE_H
