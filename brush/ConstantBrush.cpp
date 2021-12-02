/**
 * @file   ConstantBrush.cpp
 *
 * Implementation of a brush with a constant mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "ConstantBrush.h"
#include <cmath>
#include <algorithm>
#include "Canvas2D.h"

ConstantBrush::ConstantBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
}
void ConstantBrush::makeMask() {
    m_mask.resize(pow(getRadius(), 2));
    for (int y = 0; y < getRadius(); y++) {
        for (int x = 0; x < getRadius(); x++) {
            if (pow(x, 2) + pow(y, 2) < pow(getRadius(), 2)) {
                m_mask[y * getRadius() + x] = 1;
            } else {
                m_mask[y * getRadius() + x] = 0;
            }
        }
    }
}
