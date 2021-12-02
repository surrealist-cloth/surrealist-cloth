#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "Camera.h"

#include "Settings.h"
#include "SupportCanvas3D.h"
#include "ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"
using namespace CS123::GL;


SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    loadPhongShader();
    loadWireframeShader();
    loadNormalsShader();
    loadNormalsArrowShader();

    this->settingsChanged();
}

SceneviewScene::~SceneviewScene()
{
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
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
    setClearColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderPhongPass(context);

    if (settings.drawWireframe) {
        renderWireframePass(context);
    }

    if (settings.drawNormals) {
        renderNormalsPass(context);
    }
}

void SceneviewScene::setGlobalData() {
    // [TODO] pass global data to shader.vert using m_phongShader
    m_phongShader->setUniform("ka", getGlobal().ka);
    m_phongShader->setUniform("kd", getGlobal().kd);
    m_phongShader->setUniform("ks", getGlobal().ks);
}

void SceneviewScene::setSceneUniforms(SupportCanvas3D *context) {
    Camera *camera = context->getCamera();
    m_phongShader->setUniform("useLighting", settings.useLighting);
    m_phongShader->setUniform("useArrowOffsets", false);
    // m_phongShader->setUniform("useTexture", settings.useTextureMapping ? 1 : 0);
    m_phongShader->setUniform("isShapeScene", false);
    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
    m_phongShader->setUniform("v", camera->getViewMatrix());
}

void SceneviewScene::setMatrixUniforms(Shader *shader, SupportCanvas3D *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
    shader->setUniform("v", context->getCamera()->getViewMatrix());
}

void SceneviewScene::setLights()
{
    for (auto& light : m_lights) {
        m_phongShader->setLight(light);
    }
}

void SceneviewScene::renderGeometry(std::unique_ptr<CS123::GL::Shader>& shader) {
    assert(m_shapes.size() == m_primitives.size());
    assert(m_shapes.size() == m_transformations.size());
    for (int i = 0; i < m_primitives.size(); i++) {
        shader->setUniform("m", m_transformations[i]);

        m_shapes[i]->draw();
    }
}

void SceneviewScene::renderGeometry(std::unique_ptr<CS123::GL::CS123Shader>& shader) {
    assert(m_shapes.size() == m_primitives.size());
    assert(m_shapes.size() == m_transformations.size());
    for (int i = 0; i < m_primitives.size(); i++) {
        shader->setUniform("m", m_transformations[i]);
        shader->applyMaterial(m_primitives[i].material);
        m_shapes[i]->draw();
    }
}

void SceneviewScene::renderPhongPass(SupportCanvas3D *context) {
    m_phongShader->bind();
    setGlobalData();
    setSceneUniforms(context);
    setLights();
    useFill();
    renderGeometry(m_phongShader);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();
}

void SceneviewScene::renderWireframePass(SupportCanvas3D *context) {
    m_wireframeShader->bind();
    setMatrixUniforms(m_wireframeShader.get(), context);
    useWireframe();
    renderGeometry(m_wireframeShader);
    m_wireframeShader->unbind();
}

void SceneviewScene::renderNormalsPass(SupportCanvas3D *context) {
    // Render the lines.
    m_normalsShader->bind();
    setMatrixUniforms(m_normalsShader.get(), context);
    useWireframe();
    renderGeometry(m_normalsShader);
    m_normalsShader->unbind();

    // Render the arrows.
    m_normalsArrowShader->bind();
    setMatrixUniforms(m_normalsArrowShader.get(), context);
    useFill();
    renderGeometry(m_normalsArrowShader);
    m_normalsArrowShader->unbind();
}

void SceneviewScene::useWireframe()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void SceneviewScene::useFill()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void SceneviewScene::settingsChanged() {
    if (settings.shapeParameter1 != m_lastSettings.shapeParameter1 ||
        settings.shapeParameter2 != m_lastSettings.shapeParameter2 ||
        settings.shapeParameter3 != m_lastSettings.shapeParameter3) {
        Scene::setupShapes();
    }
    m_lastSettings = settings;
}

