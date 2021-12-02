#include "SobelFilter.h"
#include "RGBA.h"
#include <iostream>
#include <vector>

SobelFilter::SobelFilter()
{

}

SobelFilter::~SobelFilter()
{

}

void SobelFilter::apply(Canvas2D *canvas)
{

    RGBA* data = canvas->data();
    int width = canvas->width();
    int height = canvas->height();
    std::vector<Intensity> grayscale(width * height, 0.f);
    // initialize grayscale with grayscaled values from data
    RGBA2Intensity(data, grayscale, width * height);

    // intermediary buffer for first step in separable convolution
    std::vector<Intensity> intermediary(width * height, 0.f);

    // store outputs of horizontal and vertical sobel
    std::vector<Intensity> horizontal(width * height, 0.f);
    std::vector<Intensity> vertical(width * height, 0.f);

    // horizontal sobel
    convolveHorizontal(grayscale, intermediary, width, height, HORIZONTAL_FILTER_X);
    convolveVertical(intermediary, horizontal, width, height, HORIZONTAL_FILTER_Y);

    // vertical sobel
    convolveHorizontal(grayscale, intermediary, width, height, VERTICAL_FILTER_X);
    convolveVertical(intermediary, vertical, width, height, VERTICAL_FILTER_Y);

    // combine the two sobel outputs
    for (int i = 0; i < width * height; i++) {
        grayscale[i] = glm::min(glm::sqrt(glm::pow(horizontal[i], 2.f) + glm::pow(vertical[i], 2.f)) * settings.edgeDetectSensitivity, 1.f);
    }

    // map grayscale back to RGBA
    Intensity2RGBA(grayscale, data, width * height);
}

void SobelFilter::convolveHorizontal(std::vector<Intensity>& data, std::vector<Intensity>& output, int width, int height, const glm::vec3 &filter)
{
    int halfFilterSize = FILTER_SIZE / 2;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int outputCoordinate = getCoordinate(row, col, width);

            for (int i = -halfFilterSize; i <= halfFilterSize; i++) {
                // ignore out of bound values
                if ((i + col) < 0 || (i + col) >= width) continue;
                output[outputCoordinate] += data[outputCoordinate + i] * filter[i + halfFilterSize];
            }
        }
    }
}

void SobelFilter::convolveVertical(std::vector<Intensity>& data, std::vector<Intensity>& output, int width, int height, const glm::vec3 &filter)
{
    int halfFilterSize = FILTER_SIZE / 2;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int outputCoordinate = getCoordinate(row, col, width);

            for (int j = -halfFilterSize; j <= halfFilterSize; j++) {
                // ignore out of bound values
                if ((j + row) < 0 || (j + row) >= height) continue;
                output[outputCoordinate] += data[outputCoordinate + j * width] * filter[j + halfFilterSize];
            }
        }
    }
}
