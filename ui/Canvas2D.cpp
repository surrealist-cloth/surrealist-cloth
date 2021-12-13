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
#include "Canvas2D.h"
#include "Settings.h"
#include "RayScene.h"
#include <QtConcurrent>
#include <QList>
#include <QCoreApplication>
#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"
#include <glm/glm.hpp>
#include "RGBA.h"
#include <utility>
#include "filter/Filter.h"
#include "filter/SobelFilter.h"
#include "filter/BlurFilter.h"
#include "filter/ScaleFilter.h"

Canvas2D::Canvas2D() :
    // @TODO: Initialize any pointers in this class here.
    m_rayScene(nullptr),
    m_brush(nullptr)
{
    this->settingsChanged(); // to trigger a brush update
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
    if (settings.brushColor != m_lastSettings.brushColor ||
            settings.brushRadius != m_lastSettings.brushRadius ||
            settings.brushType != m_lastSettings.brushType) {
        // reinitialize the brush
        switch (settings.brushType) {
            case BRUSH_CONSTANT:
                m_brush.reset(new ConstantBrush(settings.brushColor, settings.brushRadius));
                break;
            case BRUSH_LINEAR:
                m_brush.reset(new LinearBrush(settings.brushColor, settings.brushRadius));
                break;
            case BRUSH_QUADRATIC:
                m_brush.reset(new QuadraticBrush(settings.brushColor, settings.brushRadius));
                break;
            case BRUSH_SMUDGE:
                m_brush.reset(new SmudgeBrush(settings.brushColor, settings.brushRadius));
                break;
            default:
                std::cerr << "Brush type " << settings.brushType << "not implemented" << std::endl;
        }
    }
    m_lastSettings = settings;
    std::cout << "Canvas2d::settingsChanged() called. Settings have changed" << std::endl;
}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************


void Canvas2D::mouseDown(int x, int y) {
    m_brush->brushDown(x, y, this, settings.fixAlphaBlending);
    std::cout << "Canvas2d::mouseDown() called" << std::endl;
}

void Canvas2D::mouseDragged(int x, int y) {
    m_brush->brushDragged(x, y, this, settings.fixAlphaBlending);
    std::cout << "Canvas2d::mouseDragged() called" << std::endl;

}

void Canvas2D::mouseUp(int x, int y) {
    m_brush->brushUp(x, y, this);
    std::cout << "Canvas2d::mouseUp() called" << std::endl;
}



// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage() {
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.
    std::unique_ptr<Filter> filter;
    switch(settings.filterType) {
        case FILTER_EDGE_DETECT:
            filter = std::make_unique<SobelFilter>();
            break;
        case FILTER_BLUR:
            filter = std::make_unique<BlurFilter>(settings.blurRadius);
            break;
        case FILTER_SCALE:
            filter = std::make_unique<ScaleFilter>(settings.scaleX, settings.scaleY);
            break;
        default:
            std::cerr << "Unhandled filter type " << settings.filterType << std::endl;
            return;
    }
    filter->apply(this);
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

void Canvas2D::renderPixel(CS123SceneCameraData *camera, const glm::mat4& invCameraTransformation, int row, int col, RGBA* pix)
{
    if (!m_isRendering) return;
    Ray ray(glm::vec3(0),
            m_rayScene->getViewplaneCoords(camera, this->width(), this->height(), VIEWPLANE_DEPTH, row, col));
    ray.transform(invCameraTransformation);

    glm::vec3 color = m_rayScene->rayTrace(ray);
    pix[row * this->width() + col] = RGBA(glm::min(255, static_cast<int>(color.x * 255)),
                                          glm::min(255, static_cast<int>(color.y * 255)),
                                          glm::min(255, static_cast<int>(color.z * 255)),
                                          255);
}

void Canvas2D::renderImage(CS123SceneCameraData *camera, int width, int height) {
    if (m_rayScene) {
        m_isRendering = true;
        resize(width, height);
        RGBA* pix = data();
        glm::mat4 invCameraTransformation = glm::inverse(m_rayScene->getCameraTransformation(camera));

        // concurrency if enabled
        if (settings.useMultiThreading) {
            QList<int> cols;

            for (int col = 0; col < this->width(); col++) {
                cols.push_back(col);
            }

            for (int row = 0; row < this->height(); row++) {
                std::cout << "Row done" << std::endl;
                QtConcurrent::blockingMap(cols, [&](int col) {
                    renderPixel(camera, invCameraTransformation, row, col, pix);
                });
                QCoreApplication::processEvents();
            }

        } else {
            for (int row = 0; row < this->height(); row++) {
                for (int col = 0; col < this->width(); col++) {
                    renderPixel(camera, invCameraTransformation, row, col, pix);
                }
                // If you want the interface to stay responsive, make sure to call
                // QCoreApplication::processEvents() periodically during the rendering.
                QCoreApplication::processEvents();
            }
        }
    }
    m_isRendering = false;
}

void Canvas2D::cancelRender() {
    // TODO: cancel the raytracer (optional)
    m_isRendering = false;
}




