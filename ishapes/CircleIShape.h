#ifndef CIRCLEISHAPE_H
#define CIRCLEISHAPE_H

#include "IShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "scenegraph/RayScene.h"

class CircleIShape : public IShape
{
public:
    CircleIShape(float y, float normalY);
    virtual ~CircleIShape() {};
    std::unique_ptr<glm::vec2> parameterize(glm::vec3& point) const override;
protected:
    std::vector<IntersectionCandidate> intersect(const Ray& ray) const override;
private:
    float m_y;
    float m_normalY;
};

#endif // CIRCLEISHAPE_H
