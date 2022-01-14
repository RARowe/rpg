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
    bool doubleClick;
    bool mouseMoving;
    Point coords;
    bool hasLastPressedKey;
    SDL_Keycode lastPressedKey;
} Input;

SDL_Event event;

int input_process(Input* i) {
    i->pressed.clear();
    i->released.clear();
    i->mouseMoving = false;
    i->doubleClick = false;
    if (i->mouseState == MOUSE_STATE_PRESSED) {
        i->mouseState = MOUSE_STATE_DOWN;
    } else if (i->mouseState == MOUSE_STATE_RELEASED) {
        i->mouseState = MOUSE_STATE_NONE;
    }
    i->hasLastPressedKey = false;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) {
            return 0;
        } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            SDL_Keycode key = event.key.keysym.sym;
            if (event.type == SDL_KEYDOWN) {
                i->pressed.insert(key);
                i->down.insert(key);
                i->lastPressedKey = key;
                i->hasLastPressedKey = true;
            } else {
                i->down.erase(key);
                i->released.insert(key);
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            i->mouseState = MOUSE_STATE_PRESSED;
            i->doubleClick = event.button.clicks > 1;
        } else if (event.type == SDL_MOUSEMOTION) {
            i->mouseMoving = true;
            i->coords.x = event.motion.x;
            i->coords.y = event.motion.y;
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            i->mouseState = MOUSE_STATE_RELEASED;
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

inline int input_get_last_pressed_key(const Input* i, char* c) {
    if (i->hasLastPressedKey) {
        *c = (char)i->lastPressedKey;
        return 1;
    }
    return 0;
}
