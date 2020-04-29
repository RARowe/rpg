#pragma once
#ifndef AUDIO_H
#define AUDIO_H
#include <map>
#include <string>
#include <SDL2/SDL_mixer.h>

#define DEFAULT_VOLUME 30
#define MAX_VOLUME 100
#define MIN_VOLUME 0

class Audio
{
    public:
        Audio();
        ~Audio();
        void play(const std::string& path);
        void playSound(const std::string& path);
        void volumeUp();
        void volumeDown();
    private:
        Mix_Music* _music = nullptr;
        int _volume = DEFAULT_VOLUME;
        std::map<const std::string, Mix_Chunk*> _sounds;
};
#endif