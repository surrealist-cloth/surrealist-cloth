/**
 * @file   Brush.cpp
 *
 * Implementation of a brush that smudges the image on the canvas as the mouse is dragged.
 *
 * You should fill this file in for the Brush assignment.
 */

#include "SmudgeBrush.h"

#include "Canvas2D.h"
#include <algorithm>
#include <cmath>

SmudgeBrush::SmudgeBrush(RGBA color, int radius) :
    QuadraticBrush(color, radius)
{
    makeMask();
}


SmudgeBrush::~SmudgeBrush()
{
}

void SmudgeBrush::brushUp(int x, int y, Canvas2D* canvas) {
}


//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask() {
    QuadraticBrush::makeMask();
}

void SmudgeBrush::brushDown(int x, int y, Canvas2D *canvas, bool fixAlphaBlending) {
    pickUpPaint(x, y, canvas);
}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas) {
    int width = canvas->width();
    int height = canvas->height();

    m_paint.resize(pow(getRadius() * 2 + 1, 2));
    RGBA* pix = canvas->data();
    for (int row = 0; row <= getRadius() * 2; row++) {
        for (int col = 0; col <= getRadius() * 2; col++) {
            int canvasY = row + y - getRadius();
            int canvasX = col + x - getRadius();
            int distanceSquare = pow(canvasY - y, 2) + pow(canvasX - x, 2);
            if (distanceSquare < pow(getRadius(), 2) &&
                    (canvasY < height && canvasY >= 0) &&
                    (canvasX < width && canvasX >= 0)) {
                m_paint[row * (getRadius() * 2 + 1) + col] = pix[canvasY * width + canvasX];
            }
        }
    }
}

void SmudgeBrush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas, bool fixAlphaBlending) {
    RGBA* pix = canvas->data();
    for (int row = std::max(0, mouseY - getRadius()); row < std::min(canvas->height(), mouseY + getRadius() + 1); row++) {
        for (int col = std::max(0, mouseX - getRadius()); col < std::min(canvas->width(), mouseX + getRadius() + 1); col++) {
            if (pow(col - mouseX, 2) + pow(row - mouseY, 2) < pow(getRadius(), 2)) {
                int y = row - mouseY + getRadius();
                int x = col - mouseX + getRadius();
                blend(&pix[row * canvas->width() + col],
                        &m_paint[y * (getRadius() * 2 + 1) + x],
                        m_mask[abs(row - mouseY) * getRadius() + abs(col - mouseX)]);
            }
        }
    }

    canvas->update();

    // now pick up paint again...
    pickUpPaint(mouseX, mouseY, canvas);

}


