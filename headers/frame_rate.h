#ifndef FRAME_RATE_H
#define FRAME_RATE_H
#include "graphics_context.h"

class FrameRate
{
    public:
        FrameRate(GraphicsContext* graphics);
        void draw(float timestep);
    private:
        GraphicsContext* _graphics;
        Uint32 _frame = 0;
        float _acc = 0.0f;
        char _frameRateBuffer[4] = {};
};
#endif
