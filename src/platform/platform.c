#include "platform.h"
#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

#include "platform_audio.c"
#include "platform_graphics.c"
#include "platform_input.c"

int main() {

    SDL_Window *window = SDL_CreateWindow(
        "Fire", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 200, 200,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        puts(SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        puts(SDL_GetError());
        return EXIT_FAILURE;
    }

    if (TTF_Init()) {
        puts(SDL_GetError());
        return EXIT_FAILURE;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)) {
        puts(Mix_GetError());
        return EXIT_FAILURE;
    }

    GameData *game_data = malloc(game_size_of_game_data());

    if (!game_data) {
        puts("Unable to allocate memory for gameplay.");
        return EXIT_FAILURE;
    }

    if (game_init(game_data)) {
        puts("Error initializing game.");
        return EXIT_FAILURE;
    }

    SDL_Event event;

    while (1) {
        SDL_PollEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            return 0;
        default:
            game_run_frame(game_data, 0, 0, 0, 0);
            continue;
        }
        SDL_RenderPresent(renderer);
    }
}
