#include "CubeIShape.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include "scenegraph/RayScene.h"

CubeIShape::CubeIShape()
{

}

std::vector<IntersectionCandidate> CubeIShape::intersect(const Ray& ray) const
{
    std::vector<IntersectionCandidate> ts;
    float t;
    glm::vec3 point;
    // top and bottom squares
    if (ray.dir.y != 0.f) {
        t = (0.5 - ray.eye.y) / ray.dir.y;
        point = ray.getPoint(t);
        if (point.x <= 0.5 && point.x >= -0.5 && point.z <= 0.5 && point.z >= -0.5) {
            ts.push_back(IntersectionCandidate(t, [](glm::vec3 point) {
                                                return glm::vec3(0, 1, 0);
                                               }));
        }
        t = (-0.5 - ray.eye.y) / ray.dir.y;
        point = ray.getPoint(t);
        if (point.x <= 0.5 && point.x >= -0.5 && point.z <= 0.5 && point.z >= -0.5) {
            ts.push_back(IntersectionCandidate(t, [](glm::vec3 point) {
                             return glm::vec3(0, -1, 0);
                            }));
        }
    }
    // front and back squares
    if (ray.dir.z != 0.f) {
        t = (0.5 - ray.eye.z) / ray.dir.z;
        point = ray.getPoint(t);
        if (point.x <= 0.5 && point.x >= -0.5 && point.y <= 0.5 && point.y >= -0.5) {
            ts.push_back(IntersectionCandidate(t, [](glm::vec3 point) {
                             return glm::vec3(0, 0, 1);
                            }));
        }
        t = (-0.5 - ray.eye.z) / ray.dir.z;
        point = ray.getPoint(t);
        if (point.x <= 0.5 && point.x >= -0.5 && point.y <= 0.5 && point.y >= -0.5) {
            ts.push_back(IntersectionCandidate(t, [](glm::vec3 point) {
                             return glm::vec3(0, 0, -1);
                            }));
        }
    }
    // left and right squares
    if (ray.dir.x != 0.f) {
        t = (0.5 - ray.eye.x) / ray.dir.x;
        point = ray.getPoint(t);
        if (point.y <= 0.5 && point.y >= -0.5 && point.z <= 0.5 && point.z >= -0.5) {
            ts.push_back(IntersectionCandidate(t, [](glm::vec3 point) {
                             return glm::vec3(1, 0, 0);
                            }));
        }
        t = (-0.5 - ray.eye.x) / ray.dir.x;
        point = ray.getPoint(t);
        if (point.y <= 0.5 && point.y >= -0.5 && point.z <= 0.5 && point.z >= -0.5) {
            ts.push_back(IntersectionCandidate(t, [](glm::vec3 point) {
                             return glm::vec3(-1, 0, 0);
                            }));
        }
    }
    return ts;
}


std::unique_ptr<glm::vec2> CubeIShape::parameterize(glm::vec3 &point) const
{
    if (glm::epsilonEqual(point.x, -0.5f, EPSILON)) {
        return std::make_unique<glm::vec2>(point.z + 0.5f, point.y - 0.5f);
    }
    if (glm::epsilonEqual(point.x, 0.5f, EPSILON)) {
        return std::make_unique<glm::vec2>(0.5f - point.z, point.y + 0.5f);
    }
    if (glm::epsilonEqual(point.y, -0.5f, EPSILON)) {
        return std::make_unique<glm::vec2>(point.x + 0.5f, point.z + 0.5f);
    }
    if (glm::epsilonEqual(point.y, 0.5f, EPSILON)) {
        return std::make_unique<glm::vec2>(0.5f + point.x, 0.5f - point.z);
    }
    if (glm::epsilonEqual(point.z, -0.5f, EPSILON)) {
        return std::make_unique<glm::vec2>(0.5f - point.x, point.y + 0.5f);
    }
    if (glm::epsilonEqual(point.z, 0.5f, EPSILON)) {
        return std::make_unique<glm::vec2>(point.x + 0.5f, point.y + 0.5f);
    }
    return std::unique_ptr<glm::vec2>{};
}
