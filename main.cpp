#include <QApplication>
#include "mainwindow.h"
#include "scenegraph/SceneTest.h"
#include "shapes/Cube.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
