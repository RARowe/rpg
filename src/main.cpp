#include <SDL2/SDL_mixer.h>
#include "game_context.h"

int main(int argc, char* args[])
{
    GameContext game;
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    Mix_Music *music = Mix_LoadMUS("audio/back_pocket.wav");
    Mix_PlayMusic(music, 1);
    Mix_VolumeMusic(30);

    game.run();

    return EXIT_SUCCESS;
}
