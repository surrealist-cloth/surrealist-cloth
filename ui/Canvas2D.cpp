/**
 * @file Canvas2D.cpp
 *
 * CS123 2-dimensional canvas. Contains support code necessary for Brush, Filter, Intersect, and
 * Ray.
 *
 * YOU WILL NEED TO FILL THIS IN!
 *
 */

// For your convenience, a few headers are included for you.
#include <assert.h>
#include <iostream>
#include <math.h>
#include <memory>
#include <unistd.h>
#include <QCoreApplication>
#include <QPainter>

#include "Canvas2D.h"
#include "Settings.h"
#include "RayScene.h"



#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"

#include "filter/Filter.h"
#include "filter/EdgeDetect.h"
#include "filter/FilterGray.h"
#include "filter/FilterInvert.h"
#include "filter/FilterIdentity.h"
#include "filter/FilterShift.h"
#include "filter/FilterBlur.h"
#include "filter/FilterScale.h"


Canvas2D::Canvas2D() :
    // @TODO: Initialize any pointers in this class here.
    m_rayScene(nullptr),
    m_brush(std::make_unique<ConstantBrush>(settings.brushColor, settings.brushRadius)),
    m_brushType(std::make_unique<int>(settings.brushType)),
    m_brushRadius(std::make_unique<int>(settings.brushRadius)),
    m_brushRGBA(std::make_unique<RGBA>(settings.brushColor))
{
    switchBrush();
}

Canvas2D::~Canvas2D()
{
}

// This is called when the canvas size is changed. You can change the canvas size by calling
// resize(...). You probably won't need to fill this in, but you can if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {
}


void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override any painting
    // events for the 2D canvas. For now, we simply call the superclass.
    SupportCanvas2D::paintEvent(e);

}

void Canvas2D::settingsChanged() {
    // TODO: Process changes to the application settings.
    //std::cout << "Canvas2d::settingsChanged() called. Settings have changed" << std::endl;

    if (!(*m_brushType == settings.brushType)) {
        m_brushType = std::make_unique<int>(settings.brushType);
        switchBrush();
    }

    if (!(*m_brushRGBA == settings.brushColor)) {
        m_brushRGBA = std::make_unique<RGBA>(settings.brushColor);
        m_brush->setRGBA(*m_brushRGBA);
    }

    if (!(*m_brushRadius == settings.brushRadius)) {
        m_brushRadius = std::make_unique<int>(settings.brushRadius);
        switchBrush();
    }

}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************


void Canvas2D::mouseDown(int x, int y) {
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a separate
    //        class for each of your brushes. Remember that you can use the static Settings
    //        object to get the currently selected brush and its parameters.

    // You're going to need to leave the alpha value on the canvas itself at 255, but you will
    // need to use the actual alpha value to compute the new color of the pixel
    m_brush->brushDown(x, y, this);
//    bool fixAlphaBlending = settings.fixAlphaBlending; // for extra/half credit
}

void Canvas2D::mouseDragged(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    m_brush->brushDragged(x, y, this);

}

void Canvas2D::mouseUp(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
}

void Canvas2D::switchBrush() {
    switch (settings.brushType) {
        case 0:
            m_brush = std::make_unique<ConstantBrush>(settings.brushColor, settings.brushRadius);
        break;
        case 1:
            m_brush = std::make_unique<LinearBrush>(settings.brushColor, settings.brushRadius);
            break;
        case 2:
            m_brush = std::make_unique<QuadraticBrush>(settings.brushColor, settings.brushRadius);
            break;
        case 3:
            m_brush = std::make_unique<SmudgeBrush>(settings.brushColor, settings.brushRadius);
            break;
        default:
            m_brush = std::make_unique<ConstantBrush>(settings.brushColor, settings.brushRadius);
        break;
    }
}


// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage() {

    std::unique_ptr<Filter> filter;

    switch(settings.filterType) {
        case FILTER_BLUR:
            filter = std::make_unique<FilterBlur>(settings.blurRadius);
            break;
        case FILTER_EDGE_DETECT:
            filter = std::make_unique<EdgeDetect>(settings.edgeDetectSensitivity);
            break;
        case FILTER_SCALE:
            filter = std::make_unique<FilterScale>(settings.scaleX, settings.scaleY);
        break;
    }

    filter->apply(this);

    this->update();

    // Leave this code here! This code ensures that the Canvas2D will be completely wiped before
    // drawing the new image.
    repaint(rect());
    QCoreApplication::processEvents();
}

// ********************************************************************************************
// ** RAY
// ********************************************************************************************

void Canvas2D::setScene(RayScene *scene) {
    m_rayScene.reset(scene);
}

void Canvas2D::renderImage(CS123SceneCameraData *camera, int width, int height) {

    if (m_rayScene) {
        m_rayScene->render(camera, width, height, this);
    }
}

void Canvas2D::cancelRender() {
    // TODO: cancel the raytracer (optional)
}




