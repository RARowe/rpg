#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "platform.h"

typedef struct {
    std::set<Key> pressed;
    std::set<Key> down;
    std::set<Key> released;
    MouseState mouseState;
    bool mouseMoving;
    Point coords;
    bool hasLastPressedKey;
    Key lastPressedKey;
} Input;

int input_process(Input* i) {
	SDL_Event event;

    i->pressed.clear();
    i->released.clear();
    i->mouseMoving = false;
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

bool input_is_pressed(const Input* i, SDL_Keycode key) {
    return (bool)i->pressed.count(key);
}

bool input_is_down(const Input* i, SDL_Keycode key) {
    return (bool)i->down.count(key);
}

bool input_is_released(const Input* i, SDL_Keycode key) {
    return (bool)i->released.count(key);
}

int input_get_last_pressed_key(const Input* i, char* c) {
    if (i->hasLastPressedKey) {
        *c = (char)i->lastPressedKey;
        return 1;
    }
    return 0;
}

void overworld_handle_input(const Input* in, PlayerInput* i) {
    memset(i, 0, sizeof(PlayerInput));
    i->up = input_is_down(in, SDLK_UP);
    i->upClick = input_is_pressed(in, SDLK_UP);
    i->down = input_is_down(in, SDLK_DOWN);
    i->downClick = input_is_pressed(in, SDLK_DOWN);
    i->left = input_is_down(in, SDLK_LEFT);
    i->leftClick = input_is_pressed(in, SDLK_LEFT);
    i->right = input_is_down(in, SDLK_RIGHT);
    i->rightClick = input_is_pressed(in, SDLK_RIGHT);
    i->select = input_is_pressed(in, SDLK_f) && !i->select;
    i->back = input_is_pressed(in, SDLK_d) && !i->back;
    i->pause = input_is_pressed(in, SDLK_RETURN) && !i->pause;
    i->r = input_is_pressed(in, SDLK_r);
    i->b = input_is_pressed(in, SDLK_b);
    i->e = input_is_pressed(in, SDLK_e);
    i->esc = input_is_pressed(in, SDLK_ESCAPE);
}

int main() {
    Mix_Music *music = NULL;
    Mix_Chunk *wave = NULL;
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) 
        return; 

    // Load our music
    music = Mix_LoadMUS("resources/welcome.wav");
    if (music == NULL)
        return;

    wave = Mix_LoadWAV("resources/start.ogg");
    if (wave == NULL)
        return;

    if ( Mix_PlayMusic( music, -1) == -1 )
        return;

    Graphics graphics;
    graphics.init("RPG", SCREEN_WIDTH, SCREEN_HEIGHT, "resources/");
    PlayerInput input;
    float lastTime = 0.0f;
    while (input_process(&i)) {
        float currentTime = ((float)SDL_GetTicks()) / 1000;
        float timeStep = currentTime - lastTime;
        lastTime = currentTime;

    while (input_process(&i)) {
        game_run_frame(&graphics, 

        graphics.present();
        SDL_Delay(1000 / Graphics::FRAME_RATE);
    }
}
