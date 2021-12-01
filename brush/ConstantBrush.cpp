/**
 * @file   ConstantBrush.cpp
 *
 * Implementation of a brush with a constant mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include <iostream>
#include "ConstantBrush.h"

ConstantBrush::ConstantBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
}

void ConstantBrush::makeMask() {

    std::unique_ptr<int> maskWidth = std::make_unique<int>(2 * m_radius + 1);
    std::unique_ptr<int> maskHeight = std::make_unique<int>(2 * m_radius + 1);

    for (int row = 0; row < *maskWidth; row++) {
        for (int col = 0; col < *maskHeight; col++) {

            if( pow((m_radius + 1), 2) >= (pow(m_radius - row, 2) + pow(m_radius - col, 2)) ) {
                m_mask[row * (2 * m_radius + 1) + col] = 1;
            } else {
                m_mask[row * (2 * m_radius + 1) + col] = 0;
            }
        }
    }

    //printMask();
}


