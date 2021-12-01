#include "Scene.h"
#include "Camera.h"
#include "CS123ISceneParser.h"
#include <iostream>

#include "glm/gtx/transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>


Scene::Scene()
{
}

Scene::Scene(Scene &scene) :
    m_global(std::make_unique<CS123SceneGlobalData>()),
    m_lights(std::vector<CS123SceneLightData>(scene.m_lights.size())),
    m_objects(std::vector<SceneObject*>(0))
{
    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
    CS123SceneGlobalData global = { 1, 1, 1, 1};
    setGlobal(global);

    // TODO [INTERSECT]
    // Make sure to copy over the lights and the scenegraph from the old scene,
    // as well as any other member variables your new scene will need.
    m_lights = scene.m_lights;

    for (SceneObject *obj: scene.m_objects) {
        SceneObject *newObj = new SceneObject();
        newObj->cMTM = obj->cMTM;
        newObj->primitive = obj->primitive;
        m_objects.push_back(newObj);
    }
}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
    for (SceneObject *p : m_objects) {
        delete p;
    }
    m_lights.clear();
    m_objects.clear();
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    //   finishParsing()

    CS123SceneGlobalData global;
    CS123SceneLightData light;

    //Task 5: load global data
    parser->getGlobalData(global);
    sceneToFill->setGlobal(global);

    //Task 6: load light data
    for (int i = 0; i < parser->getNumLights(); i++) {
        //DO NOT ADD NON-POINT lights in the evaluation for now.
        parser->getLightData(i, light);
        if (light.type == LightType::LIGHT_POINT) {
           sceneToFill->addLight(light);
        }
    }

    //Task 7: load object data
    sceneToFill->compressSceneGraph(parser->getRootNode(), 0);
}

void Scene::compressSceneGraph(CS123SceneNode *node, int depth) {
    compressSceneGraph(node, depth, glm::mat4x4());
}

void Scene::compressSceneGraph(CS123SceneNode *node,
                             int depth, glm::mat4x4 cMTM) {


    for (CS123SceneTransformation* t: node->transformations) {
        switch (t->type) {
         case TransformationType::TRANSFORMATION_TRANSLATE:
              cMTM = cMTM * glm::translate(t->translate);
            break;
         case TransformationType::TRANSFORMATION_SCALE:
              cMTM = cMTM * glm::scale(t->scale) ;
            break;
         case TransformationType::TRANSFORMATION_ROTATE:
              cMTM = cMTM * glm::rotate(t->angle, t->rotate);
            break;
         case TransformationType::TRANSFORMATION_MATRIX:
              cMTM = cMTM * t->matrix;
         break;
        }
    }

    for (CS123SceneNode *child: node->children) {
        compressSceneGraph(child, depth ++, cMTM);
    }

    if (node->primitives.size() >= 1) {

        for (CS123ScenePrimitive *primitive : node->primitives) {

            SceneObject* sceneObject = new SceneObject();

            m_objects.push_back(sceneObject);
            sceneObject->primitive = primitive;

            sceneObject->cMTM = cMTM;
        }
    }
}

void Scene::addPrimitive(CS123ScenePrimitive *scenePrimitive, glm::mat4x4 matrix) {
    SceneObject* sceneObject = new SceneObject();
    m_objects.push_back(sceneObject);
    sceneObject->primitive = scenePrimitive;
    sceneObject->cMTM = matrix;
}

void Scene::addLight(const CS123SceneLightData &sceneLight) {
    m_lights.push_back(sceneLight);
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    m_global = std::make_unique<CS123SceneGlobalData>(global);
}













void Scene::printPrimitives() {
    std::cout << std::endl;
    std::cout <<" final primitives: "<< std::endl;

    for (SceneObject* sceneObject : m_objects) {
        switch(sceneObject->primitive->type) {
            case PrimitiveType::PRIMITIVE_CONE:
                std::cout << "cone : " << glm::to_string(sceneObject->cMTM) << std::endl;
            break;
        case PrimitiveType::PRIMITIVE_CUBE:
            std::cout << "cube : " << glm::to_string(sceneObject->cMTM) << std::endl;
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            std::cout << "cylinder : " << glm::to_string(sceneObject->cMTM) << std::endl;
            break;
        case PrimitiveType::PRIMITIVE_TORUS:
            std::cout << "torus : " << glm::to_string(sceneObject->cMTM) << std::endl;
            break;
        case PrimitiveType::PRIMITIVE_SPHERE:
            std::cout << "sphere : " << glm::to_string(sceneObject->cMTM) << std::endl;
            break;
        case PrimitiveType::PRIMITIVE_MESH:
            std::cout << "mesh : " << glm::to_string(sceneObject->cMTM) << std::endl;
            break;
        }
    }

}

void Scene::printNode(CS123SceneNode *node, int numOffset) {
    printNode(node, numOffset, std::make_unique<std::string>(""));
}

void Scene::printNode(CS123SceneNode *node, int numOffset, std::unique_ptr<std::string> name) {
    std::cout << std::endl;
    std::string offset(numOffset * 4, ' ');

    std::cout << offset << *name << " node:" << std::endl;

    std::cout << offset << "transformations : " << std::endl;
    for (CS123SceneTransformation *t : node->transformations) {
        switch (t->type) {
         case 0:
             std::cout << offset << "  translate: " << glm::to_string(t->translate) << std::endl;
            break;
         case 1:
            std::cout << offset << "  scale: " << glm::to_string(t->scale) << std::endl;
            break;
         case 2:
            std::cout << offset << "  rotate: " << glm::to_string(t->rotate) << std::endl;
            std::cout << offset << "  angle: " << t->angle << std::endl;
            break;
         case 3:
            std::cout << offset << "  matrix: " << glm::to_string(t->matrix)  << std::endl;
         break;
        }
    }
    std::cout << offset << "children : " << node->children.size() << std::endl;

    std::cout << offset << "primitives of size : " << std::to_string(node->primitives.size()) << std::endl;
    for (CS123ScenePrimitive *t : node->primitives) {

        switch(t->type) {
            case PrimitiveType::PRIMITIVE_CONE:
                std::cout << offset << "type : cone" << std::endl;
            break;
        case PrimitiveType::PRIMITIVE_CUBE:
            std::cout << offset << "type : cube" << std::endl;
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            std::cout << offset << "type : cylinder" << std::endl;
            break;
        case PrimitiveType::PRIMITIVE_TORUS:
            std::cout << offset << "type : torus" << std::endl;
            break;
        case PrimitiveType::PRIMITIVE_SPHERE:
            std::cout << offset << "type : sphere" << std::endl;
            break;
        case PrimitiveType::PRIMITIVE_MESH:
            std::cout << offset << "type : mesh" << std::endl;
            break;
        }
        std::cout << offset << "  meshfile: " << t->meshfile << std::endl;
        std::cout << offset << "  material: k_d" << glm::to_string(t->material.cDiffuse) << std::endl;
    }
}

void Scene::printInfo() const {
    std::cout << "Printing Scene information: " << std::endl;
    std::cout << std::endl;

    std::cout << "  Globaldata " << std::endl;
    std::cout << "      ambient   coef : "<< m_global->ka << std::endl;
    std::cout << "      diffuse   coef : "<< m_global->kd << std::endl;
    std::cout << "      specular  coef : "<< m_global->ks << std::endl;
    std::cout << "      trnsprncy coef : "<< m_global->kt << std::endl;
    std::cout << std::endl;

    std::cout << "  Lightdata " << std::endl;
    for (int i = 0; i < m_lights.size(); i++) {
        std::cout << "      light " << m_lights[i].id << std::endl;

        printLightType(m_lights[i].type);

        std::cout << "          color    : "<< glm::to_string(m_lights[i].color) << std::endl;
        std::cout << "          function : "<< glm::to_string(m_lights[i].function) << std::endl;
        std::cout << "          pos      : "<< glm::to_string(m_lights[i].pos) << std::endl;
        std::cout << "          dir      : "<< glm::to_string(m_lights[i].dir) << std::endl;
        std::cout << "          radius   : "<< m_lights[i].radius << std::endl;
        std::cout << "          penumbra : "<< m_lights[i].penumbra << std::endl;
        std::cout << "          angle    : "<< m_lights[i].angle << std::endl;
        std::cout << "          width    : "<< m_lights[i].width << std::endl;
        std::cout << "          height   : "<< m_lights[i].height << std::endl;
    }
}

void Scene::printLightType(LightType l) const {
    switch (l) {
        case LightType::LIGHT_POINT:
            std::cout << "          type     : point light"<< std::endl;
        break;
        case LightType::LIGHT_DIRECTIONAL:
            std::cout << "          type     : directional light"<< std::endl;
        break;
        case LightType::LIGHT_SPOT:
            std::cout << "          type     : spot light"<< std::endl;
            break;
        case LightType::LIGHT_AREA:
            std::cout << "          type     :  area light"<< std::endl;
        break;
    }
}
