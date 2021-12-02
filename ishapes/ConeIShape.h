#ifndef CONEISHAPE_H
#define CONEISHAPE_H

#include "IShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "scenegraph/RayScene.h"
#include "ishapes/CircleIShape.h"

class ConeIShape : public IShape
{
public:
    static ConeIShape &shared_instance() {static ConeIShape shape; return shape;}
    ConeIShape();
    virtual ~ConeIShape() {};
    std::unique_ptr<glm::vec3> getNormal(glm::vec3& point) const override;
    std::unique_ptr<glm::vec2> parameterize(glm::vec3& point) const override;
protected:
    std::vector<float> intersect(Ray& ray) const override;
private:
    std::unique_ptr<CircleIShape> m_bottom;
};

#endif // CONEISHAPE_H
