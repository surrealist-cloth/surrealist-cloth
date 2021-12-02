#ifndef FILTER_H
#define FILTER_H

#include "RGBA.h"
#include "Canvas2D.h"

typedef float Intensity;
typedef glm::vec3 Color;

class Filter
{
public:
    Filter();
    virtual ~Filter() {};
    virtual void apply(Canvas2D* canvas) = 0;
protected:
    static int getCoordinate(int row, int col, int width) {
        return (row * width) + col;
    }
    static float byte2REAL(unsigned char c) {
        return static_cast<float>(c) / 255.f;
    }
    static float REAL2byte(float f) {
        int i = static_cast<int>((f * 255.0 + 0.5));
        return (i < 0) ? 0 : (i > 255) ? 255 : i;
    }
    static void RGBA2Color(RGBA* input, std::vector<Color>& output, int len);
    static void Color2RGBA(std::vector<Color>& input, RGBA* output, int len);
    static void RGBA2Intensity(RGBA* input, std::vector<Intensity>& output, int len);
    static void Intensity2RGBA(std::vector<Intensity>& input, RGBA* output, int len);
};

#endif // FILTER_H
