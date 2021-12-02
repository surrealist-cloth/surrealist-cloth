#include "CircleIShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include "scenegraph/RayScene.h"


CircleIShape::CircleIShape(float y, float normalY) : m_y(y), m_normalY(normalY)
{

}

std::vector<float> CircleIShape::intersect(Ray &ray) const
{
    std::vector<float> ts;
    float t;
    glm::vec3 point;
    if (ray.dir.y != 0.f) {
        t = (m_y - ray.eye.y) / ray.dir.y;
        point = ray.getPoint(t);
        if ((point.x * point.x + point.z * point.z) <= 0.25) {
            ts.push_back(t);
        }
    }
    return ts;
}

std::unique_ptr<glm::vec3> CircleIShape::getNormal(glm::vec3& point) const
{
    return std::make_unique<glm::vec3>(0, m_normalY, 0);
}

std::unique_ptr<glm::vec2> CircleIShape::parameterize(glm::vec3 &point) const
{
    return std::make_unique<glm::vec2>(glm::min(glm::max(point.x + 0.5f, 0.f), 1.f),
                                       glm::min(glm::max(point.z + 0.5f, 0.f), 1.f));
}
