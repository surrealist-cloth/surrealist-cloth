#include "ScaleFilter.h"
#include <glm/glm.hpp>


ScaleFilter::ScaleFilter(float scaleX, float scaleY) : m_scaleX(scaleX), m_scaleY(scaleY)
{
    m_radiusX = (scaleX < 1.f) ? (1.f / scaleX) : 1.f;
    m_radiusY = (scaleY < 1.f) ? (1.f / scaleY) : 1.f;

}

void ScaleFilter::apply(Canvas2D *canvas)
{
    RGBA* data = canvas->data();
    int width = canvas->width();
    int height = canvas->height();

    // initialize color with data
    std::vector<Color> color = applyRaw(data, width, height);
    int newWidth = static_cast<int>(width * m_scaleX);
    int newHeight = static_cast<int>(height * m_scaleY);
    canvas->resize(newWidth, newHeight);
    Color2RGBA(color, data, newWidth * newHeight);
}

std::vector<Color> ScaleFilter::applyRaw(RGBA* data, int width, int height)
{
    std::vector<Color> color(width * height);
    RGBA2Color(data, color, width * height);

    // scale horizontally
    int newWidth = static_cast<int>(width * m_scaleX);
    // intermediary buffer for horizontal scaling
    std::vector<Color> horizontal(newWidth * height);
    convolveHorizontal(color, horizontal, width, height);

    // scale vertically
    int newHeight = static_cast<int>(height * m_scaleY);
    color.resize(newWidth * newHeight);
    convolveVertical(horizontal, color, newWidth, height);
    return color;
}

void ScaleFilter::applyRGBA(RGBA *data, RGBA* output, int width, int height)
{
    std::vector<Color> color = applyRaw(data, width, height);
    int newWidth = static_cast<int>(width * m_scaleX);
    int newHeight = static_cast<int>(height * m_scaleY);
    Color2RGBA(color, output, newWidth * newHeight);
}

void ScaleFilter::convolveHorizontal(std::vector<Color> &data, std::vector<Color> &output, int width, int height)
{
    int newWidth = static_cast<int>(static_cast<float>(width) * m_scaleX);
    float centerShift = (1.f - m_scaleX) / (2.f * m_scaleX);
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < newWidth; col++) {
            int outputCoordinate = getCoordinate(row, col, newWidth);
            float center = static_cast<float>(col) / m_scaleX + centerShift;
            float sum = 0.f; // track the weights sum
            Color acc(0.f);

            for (int i = glm::ceil(center - m_radiusX); i <= glm::floor(center + m_radiusX); i++) {
                // ignore out of bound values
                if (i < 0 || i >= width) {
                    continue;
                }
                float filterValue = filterX(static_cast<float>(i) - center);
                sum += filterValue;
                acc += data[getCoordinate(row, i, width)] * filterValue;
            }
            output[outputCoordinate] = acc / sum;
        }
    }
}

void ScaleFilter::convolveVertical(std::vector<Color> &data, std::vector<Color> &output, int width, int height)
{
    int newHeight = static_cast<int>(static_cast<float>(height) * m_scaleY);
    float centerShift = (1.f - m_scaleY) / (2.f * m_scaleY);
    for (int row = 0; row < newHeight; row++) {
        for (int col = 0; col < width; col++) {
            int outputCoordinate = getCoordinate(row, col, width);
            float center = static_cast<float>(row) / m_scaleY + centerShift;
            float sum = 0.f; // track the weights sum
            Color acc(0.f);

            for (int j = glm::ceil(center - m_radiusY); j <= glm::floor(center + m_radiusY); j++) {
                // ignore out of bound values
                if (j < 0 || j >= height) {
                    continue;
                }
                float filterValue = filterY(static_cast<float>(j) - center);
                sum += filterValue;
                acc += data[getCoordinate(j, col, width)] * filterValue;
            }
            output[outputCoordinate] = acc / sum;
        }
    }
}

float ScaleFilter::filterX(float x)
{
    x = fabs(x);
    if (x > m_radiusX) {
        return 0;
    } else {
        return (1.f - x / m_radiusX) / m_radiusX;
    }
}


float ScaleFilter::filterY(float x)
{
    x = fabs(x);
    if (x > m_radiusY) {
        return 0;
    } else {
        return (1.f - x / m_radiusY) / m_radiusY;
    }
}

