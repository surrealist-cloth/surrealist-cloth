#ifndef SCALEFILTER_H
#define SCALEFILTER_H

#include "Filter.h"

class ScaleFilter : public Filter
{
public:
    ScaleFilter(float scaleX, float scaleY);
    void apply(Canvas2D* canvas) override;
private:
    void convolveHorizontal(std::vector<Color>& data, std::vector<Color>& output, int width, int height);
    void convolveVertical(std::vector<Color>& data, std::vector<Color>& output, int width, int height);
    float filterX(float x);
    float filterY(float x);
    float m_scaleX;
    float m_scaleY;
    float m_radiusX;
    float m_radiusY;
};

#endif // SCALEFILTER_H
