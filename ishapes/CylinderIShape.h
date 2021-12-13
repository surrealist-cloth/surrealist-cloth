#ifndef CYLINDERISHAPE_H
#define CYLINDERISHAPE_H

#include "IShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "scenegraph/RayScene.h"
#include "ishapes/CircleIShape.h"

class CylinderIShape : public IShape
{
public:
    static CylinderIShape &shared_instance() {static CylinderIShape shape; return shape;}
    CylinderIShape();
    virtual ~CylinderIShape() {};
    std::unique_ptr<glm::vec2> parameterize(glm::vec3& point) const override;
protected:
    std::vector<IntersectionCandidate> intersect(const Ray& ray) const override;
private:
    std::unique_ptr<CircleIShape> m_top;
    std::unique_ptr<CircleIShape> m_bottom;
};

#endif // CYLINDERISHAPE_H
