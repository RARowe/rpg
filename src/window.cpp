#include <SDL2/SDL.h>
#include <vector>
#include "window.h"

Window::Window(GraphicsContext& context)
{
    _renderer = context.getRenderer();
}

void Window::draw(Drawable& shape)
{
    shape.draw(_renderer);
    SDL_RenderPresent(_renderer);
}
