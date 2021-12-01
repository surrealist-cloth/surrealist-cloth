/**
 * @file   Brush.cpp
 *
 * Implementation of a brush that smudges the image on the canvas as the mouse is dragged.
 *
 * You should fill this file in for the Brush assignment.
 */

#include "SmudgeBrush.h"

#include "Canvas2D.h"


SmudgeBrush::SmudgeBrush(RGBA color, int radius) :
    QuadraticBrush(color, radius),
    m_canvasPaint(std::vector<RGBA>((radius*2+1)*(radius*2+1)))
{
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. Remember to use automatically managed memory!

    makeMask();
}


SmudgeBrush::~SmudgeBrush()
{
    // @TODO: [BRUSH] Be sure not to leak memory!  If you're using automatic memory management everywhere,
    //       this will be empty.
}

void SmudgeBrush::brushUp(int x, int y, Canvas2D* canvas) {
}


//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your brush here. For this brush you will probably want
    //        to use a quadratic distribution for the best results. Linear or Gaussian would
    //        work too, however. Feel free to paste your code from the Linear or Quadratic brushes
    //        or modify the class inheritance to be able to take advantage of one of those class's
    //        existing implementations. The choice is yours!
    //
}

void SmudgeBrush::brushDown(int x, int y, Canvas2D *canvas) {
    pickUpPaint(x, y, canvas);
}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int mouseX, int mouseY, Canvas2D* canvas) {

    m_canvasPaint = std::vector<RGBA>((m_radius*2+1)*(m_radius*2+1));

    RGBA *pix = canvas->data();

    int maskRowCounter = - std::min(mouseY - m_radius, 0);
    int maskColCounter = - std::min(mouseX - m_radius, 0);

    std::unique_ptr<int> rowStart = std::make_unique<int> (
                std::max(0, mouseY - m_radius));
    std::unique_ptr<int> rowEnd = std::make_unique<int> (
                std::min(canvas->height(), mouseY + m_radius + 1));

    std::unique_ptr<int> colStart = std::make_unique<int> (
                std::max(0, mouseX - m_radius));
    std::unique_ptr<int> colEnd = std::make_unique<int> (
                std::min(canvas->width(), mouseX + m_radius +1));

    for (int rowCounter = *rowStart; rowCounter < *rowEnd; rowCounter++) {

        for (int colCounter = *colStart; colCounter < *colEnd; colCounter++) {

            std::unique_ptr<RGBA> canvasPixel =
                    std::make_unique<RGBA>(pix[canvas->width() * rowCounter + colCounter]);

            m_canvasPaint[maskRowCounter * (2 * m_radius + 1) + maskColCounter] = *canvasPixel;

            maskColCounter++;
        }

        maskColCounter = - std::min(mouseX - m_radius, 0);
        maskRowCounter++;
    }
}

void SmudgeBrush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {

    RGBA *pix = canvas->data();


    int maskRowCounter = - std::min(mouseY - m_radius, 0);
    int maskColCounter = - std::min(mouseX - m_radius, 0);

    std::unique_ptr<int> rowStart = std::make_unique<int> (
                std::max(0, mouseY - m_radius));
    std::unique_ptr<int> rowEnd = std::make_unique<int> (
                std::min(canvas->height(), mouseY + m_radius + 1));

    std::unique_ptr<int> colStart = std::make_unique<int> (
                std::max(0, mouseX - m_radius));
    std::unique_ptr<int> colEnd = std::make_unique<int> (
                std::min(canvas->width(), mouseX + m_radius +1));

    for (int rowCounter = *rowStart; rowCounter < *rowEnd; rowCounter++) {

        for (int colCounter = *colStart; colCounter < *colEnd; colCounter++) {

            std::unique_ptr<RGBA> canvasPixel =
                    std::make_unique<RGBA>(pix[canvas->width() * rowCounter + colCounter]);

            float m = getMaskValue(maskRowCounter * (2 * m_radius + 1) + maskColCounter);
            m_color = m_canvasPaint[maskRowCounter * (2 * m_radius + 1) + maskColCounter];

            pix[canvas->width() * rowCounter + colCounter] = blend(m, *canvasPixel, m_color);

            maskColCounter++;
        }

        maskColCounter = - std::min(mouseX - m_radius, 0);
        maskRowCounter++;
    }

    canvas->update();

    pickUpPaint(mouseX, mouseY, canvas);

}


