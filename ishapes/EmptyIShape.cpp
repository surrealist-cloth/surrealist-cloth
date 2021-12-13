#include "EmptyIShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

EmptyIShape::EmptyIShape()
{

}

std::vector<IntersectionCandidate> EmptyIShape::intersect(const Ray& ray) const
{
    return std::vector<IntersectionCandidate>();
}

