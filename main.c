#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ONE_MEG 1024 * 1024

void *allocate() {
    return VirtualAlloc(NULL, ONE_MEG, MEM_COMMIT | MEM_RESERVE,
                        PAGE_READWRITE);
}

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

    allocate();

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
    TTF_CloseFont(g->font);
    TTF_Quit();
    SDL_DestroyRenderer(g->renderer);
    SDL_DestroyWindow(g->window);
    for (int i = 0; i < g->textureCache.size; i++) {
        SDL_DestroyTexture(g->textureCache.textures[i].texture);
    }
    SDL_Quit();
}
