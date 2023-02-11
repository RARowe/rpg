#include "platform.h"

#include <SDL2/SDL.h>

static void input_handle_key_event(Input *i, SDL_Event *event) {
    SDL_Keycode key = event->key.keysym.sym;
    int mask = event->type == SDL_KEYDOWN
                   ? INPUT_STATE_DOWN | INPUT_STATE_PRESSED
                   : INPUT_STATE_UP | INPUT_STATE_RELEASED;

    switch (key) {
    case SDLK_UP:
        i->game[GAME_INPUT_UP] = mask;
        break;
    case SDLK_DOWN:
        i->game[GAME_INPUT_DOWN] = mask;
        break;
    case SDLK_LEFT:
        i->game[GAME_INPUT_LEFT] = mask;
        break;
    case SDLK_RIGHT:
        i->game[GAME_INPUT_RIGHT] = mask;
        break;
    case SDLK_ESCAPE:
        i->game[GAME_INPUT_ESC] = mask;
        break;
    case SDLK_RGUI:
    case SDLK_LGUI:
        i->game[GAME_INPUT_CMD] = mask;
        break;
    case SDLK_RCTRL:
    case SDLK_LCTRL:
        i->game[GAME_INPUT_CTRL] = mask;
        break;
    default:
        if (key >= '\r' && key <= 'z') {
            i->keys[(int)key] = mask;
            i->hasLastPressedKey = true;
            i->lastPressedKey = (Key)key;
        }
        i->game[GAME_INPUT_SELECT] = i->keys[KEY_f];
        i->game[GAME_INPUT_BACK] = i->keys[KEY_d];
        break;
    }
}

static int input_process(SDL_Event *event, Input *i) {
    /* Reset input */
    i->mouseMoving = false;
    i->hasLastPressedKey = false;

    /* Remove Pressed State from Inputs */
    for (int j = 0; j < 9; j++) {
        if (input_is(i->game[j], INPUT_STATE_PRESSED)) {
            i->game[j] = INPUT_STATE_DOWN;
        } else if (input_is(i->game[j], INPUT_STATE_RELEASED)) {
            i->game[j] = INPUT_STATE_UP;
        }
    }

    if (input_is(i->mouseState, INPUT_STATE_PRESSED)) {
        i->mouseState = INPUT_STATE_DOWN;
    } else if (input_is(i->mouseState, INPUT_STATE_RELEASED)) {
        i->mouseState = INPUT_STATE_UP;
    }

    for (int j = 0; j < 128; j++) {
        if (input_is(i->keys[j], INPUT_STATE_PRESSED)) {
            i->keys[j] = INPUT_STATE_DOWN;
        } else if (input_is(i->keys[j], INPUT_STATE_RELEASED)) {
            i->keys[j] = INPUT_STATE_UP;
        }
    }

    switch (event->type) {
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
        i->mouseX = event->motion.x;
        i->mouseY = event->motion.y;
        break;
    }

    return 1;
}
