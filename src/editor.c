#include <SDL2/SDL.h>
#include <string.h>
#include "scene.h"

typedef struct {
    bool one;
    bool two;
    bool three;
} EditorInput;

EditorInput input;

void editor_handle_input(Scene* s, const SDL_Event* event) {
    // Set all flags to false
    memset(&input, 0, sizeof(EditorInput));

    if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
        bool isKeyDown = event->type == SDL_KEYDOWN;

        switch (event->key.keysym.sym) {
            case SDLK_1:
                input.one = isKeyDown;
                break;
            case SDLK_2:
                input.two = isKeyDown;
                break;
            case SDLK_3:
                input.three = isKeyDown;
                break;
            default:
                break;
        }
    }

    if (input.one) { s->toggleBackground(); }
    if (input.two) { s->toggleMidground(); }
    if (input.three) { s->toggleForeground(); }
}
