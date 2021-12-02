/**
 * @file   QuadraticBrush.cpp
 *
 * Implementation of a brush with a quadratic mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "QuadraticBrush.h"

#include <cmath>

QuadraticBrush::QuadraticBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    makeMask();
}

QuadraticBrush::~QuadraticBrush()
{
}

void QuadraticBrush::makeMask() {
    // for quadratic brushes, we need to fit a quadratic f(x) = ax^2 + bx + c to
    // f(r) = 0, f(0) = 1, f(r/2) = 1/4
    // solving, we yield
    // a = 1 / r^2
    // b = -2 / r
    // c = 1
    float a = 1.f / pow(getRadius(), 2);
    float b = - 2.f / getRadius();
    float c = 1.f;

    m_mask.resize(pow(getRadius(), 2));
    for (int y = 0; y < getRadius(); y++) {
        for (int x = 0; x < getRadius(); x++) {
            int distanceSquare = pow(x, 2) + pow(y, 2);
            if (distanceSquare < pow(getRadius(), 2)) {
                float distance = sqrt(distanceSquare);
                m_mask[y * getRadius() + x] = a * pow(distance, 2) + b * distance + c;
            } else {
                m_mask[y * getRadius() + x] = 0;
            }
        }
    }
}


