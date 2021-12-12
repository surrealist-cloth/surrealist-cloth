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
    std::unique_ptr<glm::vec2> parameterize(glm::vec3& point) const override;

    bool isInTriangle(glm::vec3 &point) const; //is a given point on the triangle?
    std::unique_ptr<glm::vec3> getSurfaceNormal() const;
protected:
    std::vector<IntersectionCandidate> intersect(Ray& ray) const override;
private:
    bool isWithinTriangle(glm::vec3 &point) const;
    std::unique_ptr<glm::vec3> m_a;
    std::unique_ptr<glm::vec3> m_b;
    std::unique_ptr<glm::vec3> m_c;

    std::unique_ptr<glm::vec3> m_n; // Surface normal
};

#endif // TRIANGLEISHAPE_H
