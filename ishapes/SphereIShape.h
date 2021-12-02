#ifndef SPHEREISHAPE_H
#define SPHEREISHAPE_H

#include "IShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "scenegraph/RayScene.h"

class SphereIShape : public IShape
{
public:
    static SphereIShape &shared_instance() {static SphereIShape shape; return shape;}
    SphereIShape();
    virtual ~SphereIShape() {};
    std::unique_ptr<glm::vec3> getNormal(glm::vec3& point) const override;
    std::unique_ptr<glm::vec2> parameterize(glm::vec3& point) const override;
protected:
    std::vector<float> intersect(Ray& ray) const override;
};

#endif // SPHEREISHAPE_H
