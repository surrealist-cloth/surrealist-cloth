/**
 * @file   LinearBrush.cpp
 *
 * Implementation of a brush with a linear mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "LinearBrush.h"

#include <math.h>
#include <algorithm>
#include <cmath>

LinearBrush::LinearBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    makeMask();
}


LinearBrush::~LinearBrush()
{
}

void LinearBrush::makeMask() {
    m_mask.resize(pow(getRadius(), 2));
    for (int y = 0; y < getRadius(); y++) {
        for (int x = 0; x < getRadius(); x++) {
            float distance = sqrt(pow(x, 2) + pow(y, 2));
            m_mask[y * getRadius() + x] = std::max(0.f, 1.f - distance / getRadius());
        }
    }
}


