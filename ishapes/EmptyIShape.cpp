#include "EmptyIShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

EmptyIShape::EmptyIShape()
{

}

std::vector<float> EmptyIShape::intersect(Ray& ray) const
{
    return std::vector<float>();
}

std::unique_ptr<glm::vec3> EmptyIShape::getNormal(glm::vec3& point) const
{
    return std::unique_ptr<glm::vec3>{};
}
