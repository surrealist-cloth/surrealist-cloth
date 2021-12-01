/**
 * @file   QuadraticBrush.cpp
 *
 * Implementation of a brush with a quadratic mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "QuadraticBrush.h"

#include <math.h>

QuadraticBrush::QuadraticBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}

QuadraticBrush::~QuadraticBrush()
{
}

void QuadraticBrush::makeMask() {
    std::unique_ptr<int> maskWidth = std::make_unique<int>(2 * m_radius + 1);
    std::unique_ptr<int> maskHeight = std::make_unique<int>(2 * m_radius + 1);

    for (int row = 0; row < *maskWidth; row++) {
        for (int col = 0; col < *maskHeight; col++) {

            if( pow((m_radius + 1), 2) >= (pow(m_radius - row, 2) + pow(m_radius - col, 2)) ) {
                float radius = static_cast<float>(m_radius);
                float distance = sqrt( pow(radius - row, 2) + pow(radius - col, 2) );

                float maskValue = pow( distance - ((-1/radius) + 1) * distance, 2)
                        - 2 * (distance - ((-1/radius) + 1) * distance) + 1;

                m_mask[row * (2 * m_radius + 1) + col] = maskValue;
            } else {
                m_mask[row * (2 * m_radius + 1) + col] = 0;
            }

        }
    }

}

