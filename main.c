#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <Windows.h>
#include <math.h>

#include "src/qdbmp.h"

#define ONE_MEG 1024 * 1024

void* allocate() {
    return VirtualAlloc(NULL, ONE_MEG, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

int main() {
    SDL_Window* window = SDL_CreateWindow(
        "Fire",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        200,
        200,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        puts(SDL_GetError());
        return(EXIT_FAILURE);
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        puts(SDL_GetError());
        return(EXIT_FAILURE);
    }

    if (TTF_Init()) {
        puts(SDL_GetError());
        return(EXIT_FAILURE);
    }

    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)) {
        puts(Mix_GetError());
        return(EXIT_FAILURE);
    }

    //allocate();

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

// int main(int argc, char* argv[]) {
//     BMP* bmp = BMP_Create(512, 512, 24);
//     for (int i = 0; i < 512; i++) {
//         for (int j = 512; j > 0; j--) {
//             BMP_SetPixelRGB(bmp, i, j, log2(i), j, sin(i) + cos(j));
//         }
//     }
//     BMP_WriteFile(bmp, "test.bmp");
//     return(EXIT_SUCCESS);
// }