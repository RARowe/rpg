#include "platform.h"

#include <SDL2/SDL.h>

#include "platform_audio.c"
#include "platform_graphics.c"
#include "platform_input.c"

int main() {
    GameData data;
    Graphics graphics;
    Audio audio;
    Input input;

    if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not be initialized! SDL Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (!graphics_init(&graphics, "RPG", SCREEN_WIDTH, SCREEN_HEIGHT, "resources/")) {
        return EXIT_FAILURE;
    }

    if (!audio_init(&audio)) {
        return EXIT_FAILURE;
    }

    bzero(&input, sizeof(Input));

    float lastTime = 0.0f;
    while (input_process(&i)) {
        float currentTime = ((float)SDL_GetTicks()) / 1000;
        float timeStep = currentTime - lastTime;
        lastTime = currentTime;

        game_run_frame(&data, &graphics, &audio, &input, timeStep);
        audio_process(&audio);
        graphics_present(&graphics);

        SDL_Delay(1000 / 60);
    }

    graphics_shutdown(&graphics);
    audio_shutdown(&audio);
}
