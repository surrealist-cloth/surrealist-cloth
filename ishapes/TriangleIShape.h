#ifndef TRIANGLEISHAPE_H
#define TRIANGLEISHAPE_H

#include "IShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "scenegraph/RayScene.h"

class TriangleIShape : public IShape
{
public:
    TriangleIShape(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c);
    virtual ~TriangleIShape() {};
    std::unique_ptr<glm::vec3> getNormal(glm::vec3& point) const override;
    std::unique_ptr<glm::vec2> parameterize(glm::vec3& point) const override;
protected:
    std::vector<float> intersect(Ray& ray) const override;
private:
    glm::vec3 &m_a;
    glm::vec3 &m_b;
    glm::vec3 &m_c;
};

#endif // TRIANGLEISHAPE_H
