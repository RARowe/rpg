#include <SDL2/SDL.h>
#include <vector>
#include "drawable.h"
#include "graphics_context.h"

#ifndef WINDOW_H
#define WINDOW_H
class Window
{
    public:
        Window(GraphicsContext& context);
        void draw(Drawable& shape);
    private:
        SDL_Renderer* _renderer;
};
#endif
