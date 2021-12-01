/**
 * @file   Brush.cpp
 *
 * Implementation of common functionality of bitmap brushes.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include <iostream>
#include <cmath>
#include "Brush.h"
#include "Canvas2D.h"

static unsigned char lerp(unsigned char a, unsigned char b, float percent) {
    float fa = static_cast<float>(a) / 255.0f;
    float fb = static_cast<float>(b) / 255.0f;
    return static_cast<unsigned char>((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}



Brush::Brush(RGBA color, int radius) :
    // Pro-tip: Initialize all variables in the initialization list
    m_color(color),
    m_mask(std::vector<float>((radius*2+1)*(radius*2+1))),
    m_radius(radius)
{
    // Pro-tip: By the time you get to the constructor body, all of the
    // member variables have already been initialized.
}


Brush::~Brush()
{
    // Pro-tip: The destructor (here) is where you free all the resources
    // you allocated during the lifetime of the class
    // Pro-tip: If you use std::unique_ptr or std::vector, you probably
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

float Brush::getMaskValue(int i) {
    return m_mask[i];
}

void Brush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {

    RGBA *pix = canvas->data();


    int maskRowCounter = - std::min(mouseY - m_radius, 0);
    int maskColCounter = - std::min(mouseX - m_radius, 0);

    int rowStart = std::max(0, mouseY - m_radius);
    int rowEnd = std::min(canvas->height(), mouseY + m_radius + 1);

    int colStart = std::max(0, mouseX - m_radius);
    int colEnd = std::min(canvas->width(), mouseX + m_radius +1);

    for (int rowCounter = rowStart; rowCounter < rowEnd; rowCounter++) {
        for (int colCounter = colStart; colCounter < colEnd; colCounter++) {

            std::unique_ptr<RGBA> canvasPixel =
                    std::make_unique<RGBA>(pix[canvas->width() * rowCounter + colCounter]);

            float m = getMaskValue(maskRowCounter * (2 * m_radius + 1) + maskColCounter);

            pix[canvas->width() * rowCounter + colCounter] = blend(m, *canvasPixel, m_color);

            maskColCounter++;
        }

        maskColCounter = - std::min(mouseX - m_radius, 0);
        maskRowCounter++;
    }

    canvas->update();
}

RGBA Brush::blend(float mask, RGBA canvasColor, RGBA brushColor) {

      //converting canvasColor.rgb fron RGBA to (0, 1)
      float canvasRed = static_cast<float>(canvasColor.r) / 255.0f;
      float canvasGreen = static_cast<float>(canvasColor.g) / 255.0f;
      float canvasBlue = static_cast<float>(canvasColor.b) / 255.0f;

      float brushRed = static_cast<float>(brushColor.r) / 255.0f;
      float brushGreen = static_cast<float>(brushColor.g) / 255.0f;
      float brushBlue = static_cast<float>(brushColor.b) / 255.0f;
      float alpha = static_cast<float>(brushColor.a) / 255.0f;


      //blend and convert back to RGBA
    RGBA finalColor = RGBA(

               255 * ((1.0f - mask * alpha) * canvasRed
                    + mask * alpha * brushRed),

                255 * ((1.0f - mask * alpha) * canvasGreen
                    + mask * alpha * brushGreen),

               255 * ((1.0f - mask * alpha) * canvasBlue
                    + mask * alpha * brushBlue),
                255
                );

    return finalColor;
}


void Brush::printMask() {
    for (int i = 0; i < 2 * (m_radius + 1); i++) {
        for (int j = 0; j < 2* (m_radius + 1); j++) {
                std::cout << m_mask[i * (2 * m_radius + 1) + j];
        }
        std::cout << std::endl;
    }
}
