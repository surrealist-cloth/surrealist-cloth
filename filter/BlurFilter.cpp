#include "BlurFilter.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

BlurFilter::BlurFilter(int blurRadius) : m_blurRadius(blurRadius)
{
    initializeGaussianKernel();
}

void BlurFilter::apply(Canvas2D *canvas)
{
    RGBA* data = canvas->data();
    int width = canvas->width();
    int height = canvas->height();
    std::vector<Color> color(width * height);

    // initialize color with data
    RGBA2Color(data, color, width * height);

    // intermediary buffer for first step in separable convolution
    std::vector<Color> intermediary(width * height);

    // convolve
    convolveHorizontal(color, intermediary, width, height);
    convolveVertical(intermediary, color, width, height);

    Color2RGBA(color, data, width * height);
}

void BlurFilter::initializeGaussianKernel()
{
    // initialize kernel
    m_kernel.resize(m_blurRadius + 1);


    // setting std to be around radius / 3 gives near 0 at the edge of the kernel
    float sigma = static_cast<float>(m_blurRadius) / 3.f;
    float sum = 0.f;
    for (int i = 0; i < m_kernel.size(); i++) {
        m_kernel[i] = gaussian(i, sigma);
        sum += m_kernel[i];
        if (i > 0) sum += m_kernel[i]; // need to double count for other side
    }

    // normalize
    for (int i = 0; i < m_kernel.size(); i++) {
        m_kernel[i] /= sum;
    }
}

float BlurFilter::gaussian(float x, float sigma)
{
    return glm::exp(- glm::pow(x, 2.f) / (2.f * glm::pow(sigma, 2.f))) / (sigma * glm::sqrt(2.f * glm::pi<float>()));
}

void BlurFilter::convolveHorizontal(std::vector<Color> &data, std::vector<Color> &output, int width, int height)
{
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int outputCoordinate = getCoordinate(row, col, width);
            float sum = 1.f; // defaults to normalized kernel
            Color acc(0.f);

            for (int i = -m_blurRadius; i <= m_blurRadius; i++) {
                // ignore out of bound values
                if ((i + col) < 0 || (i + col) >= width) {
                    sum -= m_kernel[glm::abs(i)];
                    continue;
                }
                acc += data[outputCoordinate + i] * m_kernel[glm::abs(i)];
            }
            output[outputCoordinate] = acc / sum;
        }
    }
}

void BlurFilter::convolveVertical(std::vector<Color> &data, std::vector<Color> &output, int width, int height)
{
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int outputCoordinate = getCoordinate(row, col, width);
            float sum = 1.f; // defaults to normalized kernel
            Color acc(0.f);

            for (int j = -m_blurRadius; j <= m_blurRadius; j++) {
                // ignore out of bound values but fix sums
                if ((j + row) < 0 || (j + row) >= height) {
                    sum -= m_kernel[glm::abs(j)];
                    continue;
                }
                acc += data[outputCoordinate + j * width] * m_kernel[glm::abs(j)];
            }
            output[outputCoordinate] = acc / sum;
        }
    }
}
