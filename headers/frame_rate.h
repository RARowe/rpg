#ifndef FRAME_RATE_H
#define FRAME_RATE_H
#include <SDL2/SDL.h>
#include "graphics_context.h"

class FrameRate
{
    public:
        FrameRate(GraphicsContext* graphics);
        void draw(float timestep);
    private:
        GraphicsContext* _graphics;
};
#endif
