#include <SDL2/SDL_mixer.h>
#include <iostream>
#include "audio.h"

Audio::Audio()
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    Mix_VolumeMusic(30);
}

Audio::~Audio()
{
    Mix_FreeMusic(_music);
    for (auto& keyPair : _sounds)
    {
        Mix_FreeChunk(keyPair.second);
    }
}

void Audio::play(const std::string& path)
{
    Mix_FreeMusic(_music);
    _music = Mix_LoadMUS(path.c_str());
    Mix_PlayMusic(_music, -1);
}

void Audio::playSound(const std::string& path)
{
    if (_sounds.count(path) == 0)
    {
        _sounds[path] = Mix_LoadWAV(path.c_str());
    }

    if(Mix_PlayChannel(-1, _sounds[path], 0) == -1) {
        std::cout << "Mix_PlayChannel: " << Mix_GetError() << std::endl;
    }
}