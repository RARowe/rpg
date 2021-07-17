#pragma once
#include <SDL2/SDL.h>
#include <set>

std::set<SDL_Keycode> pressed;
std::set<SDL_Keycode> down;
std::set<SDL_Keycode> released;
SDL_Event event;

int input_process() {
    pressed.clear();
    released.clear();

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return 0;
        }
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
        {
            SDL_Keycode key = event.key.keysym.sym;
            if (event.type == SDL_KEYDOWN) {
                pressed.insert(key);
                down.insert(key);
            } else {
                down.erase(key);
                released.insert(key);
            }
        }
    }

    return 1;
}

inline bool input_is_pressed(SDL_Keycode key) {
    return (bool)pressed.count(key);
}

inline bool input_is_down(SDL_Keycode key) {
    return (bool)down.count(key);
}

inline bool input_is_released(SDL_Keycode key) {
    return (bool)released.count(key);
}

