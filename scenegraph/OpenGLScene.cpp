#include "OpenGLScene.h"

#include <GL/glew.h>

#include "Settings.h"

OpenGLScene::~OpenGLScene()
{
}

/**
 * @brief OpenGLScene::setClearColor
 * Sets the background to gray if there are normals or Wireframes to be drawn,
 * else sets background to black
 */
void OpenGLScene::setClearColor() {
    if (settings.drawWireframe || settings.drawNormals) {
        glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    } else {
        glClearColor(0, 0, 0, 0);
    }
}

void OpenGLScene::settingsChanged() {

}
