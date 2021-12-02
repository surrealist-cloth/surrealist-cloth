#ifndef TRIANGLEBLURFILTER_H
#define TRIANGLEBLURFILTER_H

#include "Filter.h"
#include <glm/glm.hpp>

class BlurFilter : public Filter
{
public:
    BlurFilter(int blurRadius);
    void apply(Canvas2D* canvas) override;
private:
    void initializeGaussianKernel();
    static float gaussian(float x, float sigma);

    void convolveHorizontal(std::vector<Color>& data, std::vector<Color>& output, int width, int height);
    void convolveVertical(std::vector<Color>& data, std::vector<Color>& output, int width, int height);

    int m_blurRadius;
    std::vector<float> m_kernel;
};

#endif // TRIANGLEBLURFILTER_H
