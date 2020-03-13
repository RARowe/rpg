#include <SDL2/SDL.h>
#include "keyboard_handler.h"

void KeyboardHandler::handleEvent(SDL_KeyboardEvent& event)
{
    if (event.state == SDL_PRESSED)
    {
        _pressed.insert(event.keysym.sym);
    }
    else
    {
        _pressed.erase(event.keysym.sym);
    }
}

bool KeyboardHandler::isPressed(SDL_Keycode code)
{
    return _pressed.count(code) > 0;
}