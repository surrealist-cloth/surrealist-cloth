#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"

#include <vector>
#include <glm/glm.hpp>
#include "CS123SceneData.h"
#include <memory>
#include <utility>
#include <vector>
#include "ishapes/IShape.h"
#include "RGBA.h"
#include <functional>
#include "ishapes/IShape.h"


#define MAX_RECURSION 5
#define MIN_INTENSITY 0.0001

/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
class RayScene : public Scene {
public:
    RayScene(Scene &scene);
    virtual ~RayScene();
    glm::vec3 getViewplaneCoords(CS123SceneCameraData* camera, int width, int height, float depth, int row, int col);
    std::unique_ptr<ShapeIntersection> intersect(Ray& ray);
    glm::mat4 getCameraTransformation(CS123SceneCameraData* camera);

    glm::vec3 rayTrace(Ray& ray);
private:
    glm::vec3 rayTrace(Ray& ray, int maxRecursion);
    glm::vec3 recursePhong(glm::vec3 color, Ray &ray, ShapeIntersection &s, int maxRecursion);
    glm::vec3 recurseMetal(glm::vec3 color, Ray &ray, ShapeIntersection &s, int maxRecursion);
    glm::vec4 getTexture(ShapeIntersection& s);
    bool isOccluded(Ray& lightRay, ShapeIntersection& s, float maxT);
    glm::vec3 illuminate(ShapeIntersection& s, glm::vec3 pos);
    std::vector<std::unique_ptr<IShape>> m_ishapes;
    float EPSILON = 0.001;
};

#endif // RAYSCENE_H
