#include "platform.h"

#include <stdlib.h>

// #include "game.h"
#include "platform_audio.c"
#include "platform_graphics.c"
#include "platform_input.c"

// int main() {
//     // GameData data;
//     Graphics graphics;
//     Audio audio;
//     Input input;
//     if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO) < 0) {
//         printf("SDL could not be initialized! SDL Error: %s\n",
//         SDL_GetError()); return EXIT_FAILURE;
//     }

//     if (!audio_init(&audio)) {
//         return EXIT_FAILURE;
//     }

//     // bzero(&input, sizeof(Input));

//     // TODO: Remove this. Game should initialize
//     // game_init(&data);

//     // float lastTime = 0.0f;
//     // while (input_process(&input)) {
//     //     float currentTime = ((float)SDL_GetTicks()) / 1000;
//     //     float timeStep = currentTime - lastTime;
//     //     lastTime = currentTime;

//     //     // game_run_frame(&data, &graphics, &audio, &input, timeStep);

//     //     audio_process(&audio);
//     //     graphics_present(&graphics);

//     //     SDL_Delay(1000 / 60);
//     // }

//     // graphics_shutdown(&graphics);
//     audio_shutdown(&audio);
// }

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#define ONE_MEG 1024 * 1024

// void *allocate() {
// return VirtualAlloc(NULL, ONE_MEG, MEM_COMMIT | MEM_RESERVE,
// PAGE_READWRITE);
// }

int main() {
    SDL_Window *window = SDL_CreateWindow(
        "Fire", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 200, 200,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        puts(SDL_GetError());
        return (EXIT_FAILURE);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        puts(SDL_GetError());
        return (EXIT_FAILURE);
    }

    if (TTF_Init()) {
        puts(SDL_GetError());
        return (EXIT_FAILURE);
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)) {
        puts(Mix_GetError());
        return (EXIT_FAILURE);
    }

    //    allocate();

    SDL_Event event;

    while (1) {
        SDL_PollEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            return 0;
        default:
            continue;
        }
    }
}
