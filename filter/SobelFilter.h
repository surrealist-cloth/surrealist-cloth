#ifndef SOBELFILTER_H
#define SOBELFILTER_H

#include "Filter.h"
#include <glm/glm.hpp>
#include <vector>

class SobelFilter : public Filter
{
public:
    SobelFilter();
    virtual ~SobelFilter();
    void apply(Canvas2D* canvas) override;
private:
    void convolveHorizontal(std::vector<Intensity>& data, std::vector<Intensity>& output, int width, int height, const glm::vec3& filter);
    void convolveVertical(std::vector<Intensity>& data, std::vector<Intensity>& output, int width, int height, const glm::vec3& filter);

    const glm::vec3 HORIZONTAL_FILTER_X = glm::vec3(-1, 0, 1);
    const glm::vec3 HORIZONTAL_FILTER_Y = glm::vec3(1, 2, 1);
    const glm::vec3 VERTICAL_FILTER_X = glm::vec3(1, 2, 1);
    const glm::vec3 VERTICAL_FILTER_Y = glm::vec3(1, 0, -1);
    const size_t FILTER_SIZE = 3;
};

#endif // SOBELFILTER_H
