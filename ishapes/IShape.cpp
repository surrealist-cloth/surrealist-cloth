#include "IShape.h"
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <utility>
#include <vector>
#include <memory>
#include <cmath>
#include "scenegraph/RayScene.h"
#include <algorithm>

std::unique_ptr<float> IShape::closestIntersect(Ray& ray) const {
    std::vector<float> intersections = allIntersect(ray);
    if (intersections.size() == 0) return std::unique_ptr<float>{};
    return std::make_unique<float>(*std::min_element(intersections.begin(), intersections.end()));
}

std::vector<float> IShape::allIntersect(Ray &ray) const
{
    std::vector<float> ts;
    std::vector<float> allTs = intersect(ray);
    std::copy_if(allTs.begin(), allTs.end(), std::back_inserter(ts), [](float t) {
        return t >= 0.f;
    });
    return ts;
}

std::vector<float> IShape::solveQuadratic(float a, float b, float c) const
{
    std::vector<float> solutions;
    if (glm::epsilonEqual(a, 0.f, EPSILON)) {
        return solutions;
    }
    float determinant = b * b - 4 * a * c;
    if (determinant < 0.f) {
        return solutions;
    }
    float solution = (-b + sqrt(determinant)) / (2 * a);
    if (solution >= 0.f) {
        solutions.push_back(solution);
    }
    solution = (-b - sqrt(determinant)) / (2 * a);
    if (solution >= 0.f) {
        solutions.push_back(solution);
    }
    return solutions;
}
