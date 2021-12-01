#include "SceneTest.h"
#include <assert.h>
#include <iostream>


SceneTest::SceneTest()
{
}

void SceneTest::runTests() {
    std::cout << "Running Scene Tests" << std::endl;
    testCopyConstructor();
}


void SceneTest::testCopyConstructor() {
    Scene *scene = new Scene();

//    CS123SceneColor global = new CS123SceneColor;
//    globalData.ka = 0.5f;
//    globalData.kd = 0.5f;
//    globalData.ks = 0.5f;

//    scene->setGlobal();

    delete scene;
}

SceneTest::~SceneTest() {
}
