#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "Camera.h"
#include "shapes/OpenGLShape.h"


#include <sstream>
#include <iostream>

#include "Settings.h"
#include "SupportCanvas3D.h"
#include "ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"

#include "shapes/Cube.h"
#include "shapes/Cylinder.h"
#include "shapes/Cone.h"
#include "shapes/Sphere.h"

#include "gl/GLDebug.h"





#include "OpenGLScene.h"




using namespace CS123::GL;


SceneviewScene::SceneviewScene(): m_shape(nullptr)
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    loadPhongShader();
    loadWireframeShader();
    loadNormalsShader();
    loadNormalsArrowShader();
}

SceneviewScene::~SceneviewScene()
{
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert"); // probably fine
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadWireframeShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.frag");
    m_wireframeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadNormalsShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normals.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
    m_normalsShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::loadNormalsArrowShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.frag");
    m_normalsArrowShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::render(SupportCanvas3D *context) {
    setClearColor(); // 1 set background color (black or gray depending on normals/wireframe)

    renderPhongPass(context);

}

void SceneviewScene::renderPhongPass(SupportCanvas3D *context) {
    m_phongShader->bind(); // 2

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 3
    clearLights(); // 4      // DEBUG from ShapesScene

    setLights(context->getCamera()->getViewMatrix()); // 5

    //setSceneUniforms(context); // 6, 7 and 8

    setMatrixUniforms(m_phongShader.get(), context);


    renderGeometry(context); // 9
    //glBindTexture(GL_TEXTURE_2D, 0); // EXTRA Credit
    m_phongShader->unbind();
}

//from ShapesScene
glm::vec3 toGLMVec3(const CS123SceneColor &c) {
    return glm::vec3(c.r, c.g, c.b);
}

void SceneviewScene::setPhongSceneUniforms(SupportCanvas3D *context, CS123SceneMaterial material) { // 6
    m_phongShader->setUniform("useLighting", settings.useLighting); // 6.1
    m_phongShader->setUniform("useArrowOffsets", false); // 6.2
    m_phongShader->applyMaterial(material);
}


void SceneviewScene::setMatrixUniforms(Shader *shader, SupportCanvas3D *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix()); // 8.1
    shader->setUniform("v", context->getCamera()->getViewMatrix()); // 8.2
}

void SceneviewScene::setLights(const glm::mat4 viewMatrix) {
    // TODO: [SCENEVIEW] Fill this in...
    // Set up the lighting for your scene using m_phongShader.
    // The lighting information will most likely be stored in CS123SceneLightData structures.
    //

    for (CS123SceneLightData &light: m_lights) {
        m_phongShader->setLight(light); // does this add a light or just set the light?
    }
}

//// method from ShapesScene
void SceneviewScene::clearLights() {
    for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
        std::ostringstream os;
        os << i;
        std::string indexString = "[" + os.str() + "]"; // e.g. [0], [1], etc.
        m_phongShader->setUniform("lightColors" + indexString, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}

void SceneviewScene::renderGeometry(SupportCanvas3D *context) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // 9.1
    // TODO: [SCENEVIEW] Fill this in...
    // You shouldn't need to write *any* OpenGL in this class!
    //
    //
    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.
    // Yes, indeed, we want to call our Shapes code for this.


    // 9.2


    for (SceneObject *obj : m_objects) {

        switch(obj->primitive->type) {
            case PrimitiveType::PRIMITIVE_CONE:
                m_phongShader->setUniform("m", obj->cMTM);
                setPhongSceneUniforms(context, obj->primitive->material);
                m_shape = std::make_unique<Cone>(settings.shapeParameter1, settings.shapeParameter2);
            break;

            case PrimitiveType::PRIMITIVE_CUBE:
                m_phongShader->setUniform("m", obj->cMTM);
                setPhongSceneUniforms(context, obj->primitive->material);
                m_shape = std::make_unique<Cube>(1, 1);
                break;

            case PrimitiveType::PRIMITIVE_CYLINDER:
                m_phongShader->setUniform("m", obj->cMTM);
                setPhongSceneUniforms(context, obj->primitive->material);
                m_shape = std::make_unique<Cylinder>(settings.shapeParameter1, settings.shapeParameter2);
                break;

            case PrimitiveType::PRIMITIVE_SPHERE:
                m_phongShader->setUniform("m", obj->cMTM);
                setPhongSceneUniforms(context, obj->primitive->material);
                m_shape = std::make_unique<Sphere>(settings.shapeParameter1, settings.shapeParameter2);
                break;

            case PrimitiveType::PRIMITIVE_TORUS:
                continue;
                break;

            case PrimitiveType::PRIMITIVE_MESH:
                continue;
                break;
        }

        m_shape->draw();// 9.2
    }

}

void SceneviewScene::settingsChanged() {
    // TODO: [SCENEVIEW] Fill this in if applicable.
}

