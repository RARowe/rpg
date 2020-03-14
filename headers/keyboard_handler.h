#ifndef KEYBOARD_HANDLER_H
#define KEYBOARD_HANDLER_H
#include <SDL2/SDL.h>
#include <set>
class KeyboardHandler
{
    public:
        void handleEvent(SDL_KeyboardEvent& event);
        bool isPressed(SDL_Keycode code);
        bool isPressedAndConsume(SDL_Keycode code);
    private:
        std::set<SDL_Keycode> _pressed;
};
#endif
