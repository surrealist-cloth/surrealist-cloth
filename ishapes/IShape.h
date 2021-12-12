#ifndef ISHAPE_H
#define ISHAPE_H

#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include <memory>
#include "CS123SceneData.h"

struct Ray {
    glm::vec3 eye;
    glm::vec3 dir;

    Ray(glm::vec3 eye, glm::vec3 dir) : eye(eye), dir(dir) {}
    Ray(const Ray &ray) : eye(ray.eye), dir(ray.dir) {}

    void transform(glm::mat4 transformation) {
        eye = glm::vec3(transformation * glm::vec4(eye, 1));
        dir = glm::vec3(transformation * glm::vec4(dir, 0));
    }

    glm::vec3 getPoint(float t) const {
        return eye + t * dir;
    }
};

struct IntersectionCandidate {
   IntersectionCandidate(float t,
                         std::function<glm::vec3(glm::vec3)> getNormal):
       t(t), getNormal(getNormal) {

   }
  float t;
  std::function<glm::vec3(glm::vec3)> getNormal;
};


class IShape
{
public:
    IShape() {};
    virtual ~IShape() {};
    std::unique_ptr<IntersectionCandidate> closestIntersect(Ray& ray) const;
    virtual std::unique_ptr<glm::vec2> parameterize(glm::vec3& point) const {
        return std::unique_ptr<glm::vec2>{};
    }
    std::vector<IntersectionCandidate> allIntersect(Ray& ray) const;
protected:
    virtual std::vector<IntersectionCandidate> intersect(Ray& ray) const {
        return std::vector<IntersectionCandidate>();
    }
    std::vector<float> solveQuadratic(float a, float b, float c) const;
    const float EPSILON = 0.0001;
};

struct ShapeIntersection {
    float t;
    glm::vec3 intersection;
    glm::vec3 normal;
    const CS123ScenePrimitive primitive;
    const IShape& ishape;
    const glm::mat4 invTransformation;

    ShapeIntersection(float t, glm::vec3& intersection, glm::vec3& normal, const CS123ScenePrimitive& primitive, const IShape& ishape, const glm::mat4& invTransformation): t(t), intersection(intersection), normal(normal), primitive(primitive), ishape(ishape), invTransformation(invTransformation) {}
};



#endif // ISHAPE_H
