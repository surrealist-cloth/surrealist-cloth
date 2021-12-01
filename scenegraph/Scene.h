#ifndef SCENE_H
#define SCENE_H

#include "CS123SceneData.h"
#include <map>

class Camera;
class CS123ISceneParser;

/**
 * @brief The cMTMPrimitive struct holds a primitive
 * along with its cumulative transformation matrix to be
 * directly transformed to its ultimate position in the scene.
 */
struct SceneObject {
    CS123ScenePrimitive *primitive;
    glm::mat4x4 cMTM; // cumulative transformation matrix for each object
};

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

    //prints the parsed information (or nothing)
    void printInfo() const;


    // Adds a primitive to the scene.
    virtual void addPrimitive(CS123ScenePrimitive *scenePrimitive, glm::mat4x4  matrix);

    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData &sceneLight); // this is a referance: here we are talking about type not doing an operation

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData &global);

protected:

    std::unique_ptr<CS123SceneGlobalData> m_global;
    std::vector<CS123SceneLightData> m_lights;
    std::vector<SceneObject*> m_objects; // primitives that all have a CMTM

    void compressSceneGraph(CS123SceneNode* node, int depth);
    void compressSceneGraph(CS123SceneNode* node,
                                   int depth, glm::mat4x4 cMTM);

    void printPrimitives();
    void printLightType(LightType l) const;
    static void printNode(CS123SceneNode *node, int offSet);
    static void printNode(CS123SceneNode *node, int offSet, std::unique_ptr<std::string> name);
};

#endif // SCENE_H
