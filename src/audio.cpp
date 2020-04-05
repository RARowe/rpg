#include <SDL2/SDL_mixer.h>
#include "audio.h"

Audio::Audio()
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    Mix_VolumeMusic(30);
}

Audio::~Audio()
{
    Mix_FreeMusic(_music);
}

void Audio::play(const std::string& path)
{
    Mix_FreeMusic(_music);
    _music = Mix_LoadMUS(path.c_str());
    Mix_PlayMusic(_music, -1);
}