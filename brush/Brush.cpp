/**
 * @file   Brush.cpp
 *
 * Implementation of common functionality of bitmap brushes.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "Brush.h"
#include "Canvas2D.h"
#include <algorithm>
#include <cmath>

static unsigned char lerp(unsigned char a, unsigned char b, float percent) {
    float fa = static_cast<float>(a) / 255.0f;
    float fb = static_cast<float>(b) / 255.0f;
    return static_cast<unsigned char>((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}



Brush::Brush(RGBA color, int radius) :
    // Pro-tip: Initialize all variables in the initialization list
    m_color(color),
    m_radius(radius)
{
    // Pro-tip: By the time you get to the constructor body, all of the
    // member variables have already been initialized.
}


Brush::~Brush()
{
    // Pro-tip: The destructor (here) is where you free all the resources
    // you allocated during the lifetime of the class
    // Pro-tip: If you use std::unique_ptr or std::vector, you probabably
    // won't need to put anything in here.
}

int Brush::getAlpha() const {
    return m_color.a;
}

RGBA Brush::getRGBA() const {
    return m_color;
}

int Brush::getBlue() const {
    return m_color.b;
}

int Brush::getGreen() const {
    return m_color.g;
}

int Brush::getRadius() const {
    return m_radius;
}

int Brush::getRed() const {
    return m_color.r;
}

void Brush::setAlpha(int alpha) {
    m_color.a = alpha;
}

void Brush::setRGBA(const RGBA &rgba) {
    m_color = rgba;
}

void Brush::setBlue(int blue) {
    m_color.b = blue;
}

void Brush::setGreen(int green) {
    m_color.g = green;
}

void Brush::setRed(int red) {
    m_color.r = red;
}

void Brush::setRadius(int radius) {
    m_radius = radius;
    makeMask();
}

int Brush::blendColors(int originalColor, int newColor, float maskRatio, int alpha) {
    float alphaRatio = alpha / 255.f;
    return int(originalColor * (1 - maskRatio * alphaRatio) + newColor * maskRatio * alphaRatio);
}

void Brush::blend(RGBA* original, float maskRatio) {
    original->r = blendColors(original->r, getRed(), maskRatio, getAlpha());
    original->g = blendColors(original->g, getGreen(), maskRatio, getAlpha());
    original->b = blendColors(original->b, getBlue(), maskRatio, getAlpha());
}

void Brush::blend(RGBA* original, RGBA* paint, float maskRatio) {
    original->r = blendColors(original->r, paint->r, maskRatio, paint->a);
    original->g = blendColors(original->g, paint->g, maskRatio, paint->a);
    original->b = blendColors(original->b, paint->b, maskRatio, paint->a);
}

RGBA Brush::blend(RGBA original, float maskRatio) {
    blend(&original, maskRatio);
    return original;
}

void Brush::brushDown(int x, int y, Canvas2D *canvas, bool fixAlphaBlending) {
    if (!fixAlphaBlending) {
        return;
    }
    m_alphas.resize(canvas->width() * canvas->height());
    std::fill(m_alphas.begin(), m_alphas.end(), 0.f);
    m_canvas = std::vector<RGBA>(canvas->data(), canvas->data() + canvas->width() * canvas->height());
}

void Brush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas, bool fixAlphaBlending) {
    if (fixAlphaBlending && m_alphas.size() == 0) {
        brushDown(mouseX, mouseY, canvas, fixAlphaBlending);
    }
    RGBA* pix = canvas->data();
    for (int row = std::max(0, mouseY - getRadius()); row < std::min(canvas->height(), mouseY + getRadius() + 1); row++) {
        for (int col = std::max(0, mouseX - getRadius()); col < std::min(canvas->width(), mouseX + getRadius() + 1); col++) {
            if (pow(col - mouseX, 2) + pow(row - mouseY, 2) < pow(getRadius(), 2)) {
                float maskRatio = m_mask[abs(row - mouseY) * getRadius() + abs(col - mouseX)];
                if (fixAlphaBlending) {
                    maskRatio = std::min(maskRatio + m_alphas[row * canvas->width() + col], 1.f);
                    pix[row * canvas->width() + col] = blend(m_canvas[row * canvas->width() + col], maskRatio);
                    m_alphas[row * canvas->width() + col] = maskRatio;
                } else {
                    blend(&pix[row * canvas->width() + col], maskRatio);
                }
            }
        }
    }
    canvas->update();
}
