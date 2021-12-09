#ifndef MESHISHAPE_H
#define MESHISHAPE_H

#include "IShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "scenegraph/RayScene.h"
#include "ishapes/TriangleIShape.h"

class MeshIShape : public IShape
{
public:
    MeshIShape();
    virtual ~MeshIShape() {};
    std::unique_ptr<glm::vec3> getNormal(glm::vec3& point) const override;
    std::unique_ptr<glm::vec2> parameterize(glm::vec3& point) const override;
protected:
    std::vector<float> intersect(Ray& ray) const override;
private:
    std::vector<std::unique_ptr<TriangleIShape>> m_triangles;
};

#endif // MESHISHAPE_H
