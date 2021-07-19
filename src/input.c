#pragma once
#include <SDL2/SDL.h>
#include <set>
#include "types.h"

typedef enum {
    MOUSE_STATE_NONE,
    MOUSE_STATE_PRESSED,
    MOUSE_STATE_DOWN,
    MOUSE_STATE_RELEASED
} MouseState;

typedef struct {
    std::set<SDL_Keycode> pressed;
    std::set<SDL_Keycode> down;
    std::set<SDL_Keycode> released;
    MouseState mouseState;
    bool mouseMoving;
    Point pressedMouseCoords;
    Point currentMouseCoords;
    Point releasedMouseCoords;
    bool windowResized;
    int x, y;
} Input;

SDL_Event event;

int input_process(Input* i) {
    i->pressed.clear();
    i->released.clear();
    i->mouseMoving = false;
    i->windowResized = false;
    if (i->mouseState == MOUSE_STATE_PRESSED) {
        i->mouseState = MOUSE_STATE_DOWN;
    } else if (i->mouseState == MOUSE_STATE_RELEASED) {
        i->mouseState = MOUSE_STATE_NONE;
    }

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) {
            return 0;
        } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            SDL_Keycode key = event.key.keysym.sym;
            if (event.type == SDL_KEYDOWN) {
                i->pressed.insert(key);
                i->down.insert(key);
            } else {
                i->down.erase(key);
                i->released.insert(key);
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            i->mouseState = MOUSE_STATE_PRESSED;
            i->pressedMouseCoords.x = event.motion.x;
            i->pressedMouseCoords.y = event.motion.y;
        } else if (event.type == SDL_MOUSEMOTION) {
            i->mouseMoving = true;
            i->currentMouseCoords.x = event.motion.x;
            i->currentMouseCoords.y = event.motion.y;
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            i->mouseState = MOUSE_STATE_RELEASED;
            i->releasedMouseCoords.x = event.motion.x;
            i->releasedMouseCoords.y = event.motion.y;
        } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
            i->windowResized = true;
            i->x = event.window.data1;
            i->y = event.window.data2;
        }
    }

    return 1;
}

inline bool input_is_pressed(const Input* i, SDL_Keycode key) {
    return (bool)i->pressed.count(key);
}

inline bool input_is_down(const Input* i, SDL_Keycode key) {
    return (bool)i->down.count(key);
}

inline bool input_is_released(const Input* i, SDL_Keycode key) {
    return (bool)i->released.count(key);
}
