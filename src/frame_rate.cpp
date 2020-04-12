#include <stdio.h>
#include "frame_rate.h"

FrameRate::FrameRate(GraphicsContext* graphics)
{
    _graphics = graphics;
}

void FrameRate::draw(float timestep)
{
    _acc += timestep;
    if (_frame % 10 == 0)
    {
        int frameRate = 10.0f / _acc;
        sprintf(_frameRateBuffer, "%d", frameRate);
        _acc = 0.0f;
    }
    _frame++;
    _graphics->drawText(0, 0, 60, 30, _frameRateBuffer);
}
