#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "RGBA.h"
#include "Canvas2D.h"

#include <vector>


/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
class RayScene : public Scene {
public:
    RayScene(Scene &scene);
    virtual ~RayScene();
    void render(CS123SceneCameraData *camera, int width, int height, Canvas2D* canvas);

private:
    RGBA trace(glm::vec4 p, glm::vec4 d);
    RGBA finalIntensity(glm::vec4 objAmbient, glm::vec4 objDiffuse, glm::vec4 point, glm::vec4 normal);

    glm::vec4 getNormal(glm::vec4 p, SceneObject *obj);
    float intersects(glm::vec4 p, glm::vec4 d, SceneObject *obj);

};

#endif // RAYSCENE_H
