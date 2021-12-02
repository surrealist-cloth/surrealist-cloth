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
    std::unique_ptr<glm::vec3> getNormal(glm::vec3& point) const override;
protected:
    std::vector<float> intersect(Ray& ray) const override;
};

EmptyIShape emptyIShape();

#endif // EMPTYISHAPE_H
