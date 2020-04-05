#pragma once
#ifndef AUDIO_H
#define AUDIO_H
#include <string>
#include <SDL2/SDL_mixer.h>

class Audio
{
    public:
        Audio();
        ~Audio();
        void play(const std::string& path);
    private:
        Mix_Music* _music = nullptr;
};
#endif