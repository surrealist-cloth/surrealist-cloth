#ifndef SCENE_H
#define SCENE_H

#include "CS123SceneData.h"
#include <vector>
#include <glm/glm.hpp>

class Camera;
class CS123ISceneParser;
#include <vector>
#include <memory>
#include "shapes/OpenGLShape.h"

/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */
class Scene {
public:
    Scene();
    Scene(Scene &scene);
    virtual ~Scene();

    virtual void settingsChanged() {}

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);

    const CS123SceneGlobalData& getGlobal() { return m_global; }
    const std::vector<CS123SceneLightData>& getLights() { return m_lights; };
    const std::vector<CS123ScenePrimitive>& getPrimitives() { return m_primitives; };
    const std::vector<glm::mat4>& getTransformations() { return m_transformations; };
    const std::vector<glm::mat4>& getInverseTransformations() { return m_inverseTransformations; };


protected:

    // Adds a primitive to the scene.
    virtual void addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix);

    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData &sceneLight);

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData &global);

    virtual void print();

    void setupShapes();

    CS123SceneGlobalData m_global;
    std::vector<CS123SceneLightData> m_lights;
    std::vector<CS123ScenePrimitive> m_primitives;
    std::vector<glm::mat4x4> m_transformations;
    std::vector<glm::mat4x4> m_inverseTransformations;
    std::vector<std::unique_ptr<OpenGLShape>> m_shapes;

private:
    static void dfsNode(Scene* sceneToFill, CS123SceneNode* node, glm::mat4x4 matrix);


};

#endif // SCENE_H
