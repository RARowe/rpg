#include <stdio.h>
#include "frame_rate.h"

FrameRate::FrameRate(GraphicsContext* graphics)
{
    _graphics = graphics;
}

void FrameRate::draw(float timestep)
{
    SDL_Rect out = {0, 0, 60, 30};
    char frameRateBuffer[4];
    int frameRate = 1.0f / timestep;
    sprintf(frameRateBuffer, "%d", frameRate);
    SDL_Texture* texture = _graphics->getFontTexture(frameRateBuffer);
    SDL_RenderCopy(_graphics->getRenderer(), texture, NULL, &out);
    SDL_DestroyTexture(texture);
}
