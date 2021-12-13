#include "Scene.h"
#include "Camera.h"
#include "CS123ISceneParser.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "lib/CS123SceneData.h"
#include "shapes/ConeShape.h"
#include "shapes/CubeShape.h"
#include "shapes/CylinderShape.h"
#include "shapes/SphereShape.h"
#include "shapes/EmptyShape.h"
#include "Settings.h"
#include <algorithm>

Scene::Scene()
{
}

Scene::Scene(Scene &scene)
{
    // Make sure to copy over the lights and the scenegraph from the old scene,
    // as well as any other member variables your new scene will need.
    m_global = scene.m_global;
    m_lights = scene.m_lights;
    m_primitives = scene.m_primitives;
    m_transformations = scene.m_transformations;
    m_inverseTransformations = scene.m_inverseTransformations;
    print();
}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    // load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    //   finishParsing()
    // load global data
    CS123SceneGlobalData global;
    parser->getGlobalData(global);
    sceneToFill->setGlobal(global);

    // load light data
    CS123SceneLightData light;
    for (int i = 0; i < parser->getNumLights(); i++) {
        parser->getLightData(i, light);
        sceneToFill->addLight(light);
    }

    // load primitives by DFS'ing the node tree
    CS123ScenePrimitive primitive;
    glm::mat4x4 transformation(1.0f);
    Scene::dfsNode(sceneToFill, parser->getRootNode(), transformation);
    sceneToFill->setupShapes();
}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {
    m_primitives.push_back(scenePrimitive);
    m_transformations.push_back(matrix);
    m_inverseTransformations.push_back(glm::inverse(matrix));
}

void Scene::addLight(const CS123SceneLightData &sceneLight) {
    m_lights.push_back(sceneLight);
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    m_global = global;
}

void Scene::print() {
    std::cout << "GLOBAL DATA" << std::endl;
    std::cout << "[Ambient] " << m_global.ka << " | [Diffuse] " << m_global.kd
              <<" | [Specular] " << m_global.ks << " | [Transparent] " << m_global.kt << std::endl << std::endl;
    std::cout << "LIGHTS (n=" << m_lights.size() << ")" << std::endl;
    for (auto& light : m_lights) {
        switch (light.type) {
            case LightType::LIGHT_AREA:
                std::cout << "Area light (" << light.id << "): [color] " << glm::to_string(light.color)
                          << " | [width] " << light.width << " | [height] " << light.height << std::endl;
                break;
            case LightType::LIGHT_DIRECTIONAL:
                std::cout << "Directional light (" << light.id << "): [color] " << glm::to_string(light.color)
                          << " | [pos] " << glm::to_string(light.pos) << std::endl;
                break;
            case LightType::LIGHT_POINT:
                std::cout << "Point light (" << light.id << "): [color] " << glm::to_string(light.color)
                          << " | [dir] " << glm::to_string(light.dir) << std::endl;
                break;
            case LightType::LIGHT_SPOT:
                std::cout << "Spot light (" << light.id << "): [color] " << glm::to_string(light.color)
                          << " | [radius] " << light.radius << " | [penumbra] " << light.penumbra
                          << " | [angle] " << light.angle << std::endl;
                break;
            default:
                break;
        }
    }
    std::cout << std::endl;

    std::cout << "PRIMITIVES (n=" << m_primitives.size() << ")" << std::endl;
    for (int i = 0; i < m_primitives.size(); i++) {
        auto& primitive = m_primitives[i];
        auto& transformation = m_transformations[i];

        switch (primitive.type) {
            case PrimitiveType::PRIMITIVE_CONE:
                std::cout << "Cone: [transformation] " << glm::to_string(transformation) << std::endl;
                break;
            case PrimitiveType::PRIMITIVE_CUBE:
                std::cout << "Cube: [transformation] " << glm::to_string(transformation) << std::endl;
                break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                std::cout << "Cylinder: [transformation] " << glm::to_string(transformation) << std::endl;
                break;
            case PrimitiveType::PRIMITIVE_MESH:
                std::cout << "Mesh: [file] " << primitive.meshfile << " | [transformation] " << glm::to_string(transformation) << std::endl;
                break;
            case PrimitiveType::PRIMITIVE_SPHERE:
                std::cout << "Sphere: [transformation] " << glm::to_string(transformation) << std::endl;
                break;
            case PrimitiveType::PRIMITIVE_TORUS:
                std::cout << "TORUS: [transformation] " << glm::to_string(transformation) << std::endl;
                break;
            default:
                break;
        }
    }
    std::cout << std::endl;
}

void Scene::dfsNode(Scene *sceneToFill, CS123SceneNode *node, glm::mat4x4 matrix) {
    // accumulate the node's transformations
    for (CS123SceneTransformation* transformation : node->transformations) {
        switch (transformation->type) {
            case TRANSFORMATION_TRANSLATE:
                matrix = glm::translate(matrix, transformation->translate);
                break;
            case TRANSFORMATION_ROTATE:
                matrix = glm::rotate(matrix, transformation->angle, transformation->rotate);
                break;
            case TRANSFORMATION_SCALE:
                matrix = glm::scale(matrix, transformation->scale);
                break;
            case TRANSFORMATION_MATRIX:
                matrix = transformation->matrix * matrix;
                break;
            default:
                std::cerr << "Unrecognized transformation type " << transformation->type << std::endl;
        }
    }

    // add all primitives
    for (CS123ScenePrimitive* primitive : node->primitives) {
        sceneToFill->addPrimitive(*primitive, matrix);
    }

    // dfs the children
    for (CS123SceneNode* child : node->children) {
        dfsNode(sceneToFill, child, matrix);
    }
}

void Scene::setupShapes()
{
    m_shapes.clear();
    for (auto& primitive : m_primitives) {
        std::unique_ptr<OpenGLShape> shape = std::make_unique<OpenGLShape>();
        switch (primitive.type) {
            case PrimitiveType::PRIMITIVE_CONE:
                shape->loadShape(std::make_unique<ConeShape>(std::max(3, settings.shapeParameter2), std::max(1, settings.shapeParameter1)));
                break;
            case PrimitiveType::PRIMITIVE_CUBE:
                shape->loadShape(std::make_unique<CubeShape>(std::max(1, settings.shapeParameter1)));
                break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                shape->loadShape(std::make_unique<CylinderShape>(std::max(3, settings.shapeParameter2), std::max(1, settings.shapeParameter1)));
                break;
            case PrimitiveType::PRIMITIVE_SPHERE:
                shape->loadShape(std::make_unique<SphereShape>(std::max(3, settings.shapeParameter2), std::max(2, settings.shapeParameter1)));
                break;
            default:
                shape->loadShape(std::make_unique<EmptyShape>());
                std::cerr << "Unimplemented shape type" << std::endl;
                break;
        }
        if (shape->isLoaded()) {
            m_shapes.push_back(std::move(shape));
        }
    }
}
