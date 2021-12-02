#include "Filter.h"
#include <vector>
#include <glm/glm.hpp>

Filter::Filter()
{

}


void Filter::RGBA2Color(RGBA *input, std::vector<Color> &output, int len)
{
    for (int i = 0; i < len; i++) {
        output[i].x = static_cast<float>(input->r) / 255.f;
        output[i].y = static_cast<float>(input->g) / 255.f;
        output[i].z = static_cast<float>(input->b) / 255.f;
        input++;
    }
}

void Filter::Color2RGBA(std::vector<Color> &input, RGBA *output, int len)
{
    for (int i = 0; i < len; i++) {
        output->r = glm::clamp(input[i].x * 256.f, 0.f, 255.f);
        output->g = glm::clamp(input[i].y * 256.f, 0.f, 255.f);
        output->b = glm::clamp(input[i].z * 256.f, 0.f, 255.f);
        output++;
    }
}

inline Intensity RGBAToGray(const RGBA &pixel) {
    return 0.299 * static_cast<Intensity>(pixel.r) / 255.f + 0.587 * static_cast<Intensity>(pixel.g) / 255.f + 0.114 * static_cast<Intensity>(pixel.b) / 255.f;
}


void Filter::RGBA2Intensity(RGBA *input, std::vector<Intensity> &output, int len)
{
    for (int i = 0; i < len; i++) {
        output[i] = RGBAToGray(*input);
        input++;
    }
}

void Filter::Intensity2RGBA(std::vector<Intensity>& input, RGBA *output, int len)
{
    for (int i = 0; i < len; i++) {
        unsigned char value = glm::clamp(input[i] * 255.f, 0.f, 255.f);
        output->r = value;
        output->g = value;
        output->b = value;
        output++;
    }
}
