#include "platform.h"

#include <SDL2/SDL.h>

static void input_handle_key_event(SDL_Event* event);

static int
input_process(Input* i) {
	SDL_Event event;
    
    /* Resut input */
    i->mouseMoving = false;
    i->hasLastPressedKey = false;

    /* Remove Pressed State from Inputs */
    for (int i = 0; i < 9; i++) {
        if (input_is(i->game[i], INPUT_STATE_PRESSED)) {
            i->game[i] = INPUT_STATE_DOWN;
        } else if (input_is(i->game[i], INPUT_STATE_RELEASED)) {
            i->game[i] = INPUT_STATE_UP;
        }
    }

    if (input_is(i->mouseState, INPUT_STATE_PRESSED)) {
        i->mouseState = INPUT_STATE_DOWN;
    } else if (input_is(i->mouseState, INPUT_STATE_RELEASED)) {
        i->mouseState = INPUT_STATE_UP;
    }


    for (int i = 0; i < 128; i++) {
        if (input_is(i->keys[i], INPUT_STATE_PRESSED)) {
            i->keys[i] = INPUT_STATE_DOWN;
        } else if (input_is(i->keys[i], INPUT_STATE_RELEASED)) {
            i->keys[i] = INPUT_STATE_UP;
        }
    }

    /* Poll new input */
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return 0;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                input_handle_key_event(i, &event);
                break;
            case SDL_MOUSEBUTTONDOWN:
                i->mouseState = INPUT_STATE_PRESSED | INPUT_STATE_DOWN;
                break;
            case SDL_MOUSEBUTTONUP:
                i->mouseState = INPUT_STATE_RELEASED | INPUT_STATE_UP;
                break;
            case SDL_MOUSEMOTION:
                i->mouseMoving = true;
                i->mouseX = event.motion.x;
                i->mouseY = event.motion.y;
                break;
        }
    }

    return 1;
}

inline static void
input_handle_key_event(Input* i, SDL_Event* event) {
    SDL_Keycode key = event.key.keysym.sym;
    int mask = event.type == SDL_KEYDOWN ?
        INPUT_STATE_DOWN | INPUT_STATE_PRESSED :
        INPUT_STATE_UP | INPUT_STATE_RELEASED;

    switch (key) {
        case SDLK_UP:
            i->up = mask;
            break;
        case SDLK_DOWN:
            i->down = mask;
            break;
        case SDLK_LEFT:
            i->left = mask;
            break;
        case SDLK_RIGHT:
            i->right = mask;
            break;
        case SDLK_ESCAPE:
            i->esc = mask;
            break;
        case SDLK_RGUI:
        case SDLK_LGUI:
            i->cmd = mask;
            break;
        case SDLK_RCTRL:
        case SDLK_LCTRL:
            i->ctrl = mask;
            break;
        default:
            if (key >= '\r' && key <= 'z') {
                i->keys[(int)key] = mask;
                i->hasLastPressedKey = true;
                i->lastPressedKey = (Key)key;
            } 
            i->select = i->keys[KEY_f];
            i->back = i->keys[KEY_d];
            break;
    }
}

