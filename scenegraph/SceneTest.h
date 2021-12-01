#ifndef SCENETEST_H
#define SCENETEST_H

#include "CS123SceneData.h"
#include "scenegraph/Scene.h"
#include "scenegraph/RayScene.h"


class SceneTest
{
public:
    SceneTest();
    ~SceneTest();
    static void runTests();

private:
    static void testCopyConstructor();
};

#endif // SCENETEST_H
