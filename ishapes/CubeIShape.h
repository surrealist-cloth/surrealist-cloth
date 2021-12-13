#ifndef CUBEISHAPE_H
#define CUBEISHAPE_H

#include "IShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "scenegraph/RayScene.h"

class CubeIShape : public IShape
{
public:
    static CubeIShape &shared_instance() {static CubeIShape shape; return shape;}
    CubeIShape();
    virtual ~CubeIShape() {};
    std::unique_ptr<glm::vec2> parameterize(glm::vec3& point) const override;
protected:
    std::vector<IntersectionCandidate> intersect(const Ray& ray) const override;
};

#endif // CUBEISHAPE_H
