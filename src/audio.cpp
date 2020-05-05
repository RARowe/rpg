#include <SDL2/SDL_mixer.h>
#include <iostream>
#include "audio.h"

Audio::Audio()
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    Mix_VolumeMusic(_volume);
    _pauseMenuMusic = Mix_LoadWAV("audio/pause_menu_song.wav");
    Mix_ReserveChannels(1); // Reserve one channel for pause music
}

Audio::~Audio()
{
    Mix_FreeMusic(_music);
    for (auto& keyPair : _sounds)
    {
        Mix_FreeChunk(keyPair.second);
    }
    Mix_FreeChunk(_pauseMenuMusic);
}

void Audio::play(const std::string& path)
{
    Mix_FreeMusic(_music);
    _music = Mix_LoadMUS(path.c_str());
    Mix_PlayMusic(_music, -1);
}

void Audio::playPauseMenuMusic(bool play)
{
    if (play)
    {
        Mix_PauseMusic();
        Mix_PlayChannel(PAUSE_MUSIC_CHANNEL, _pauseMenuMusic, -1);
    }
    else
    {
        Mix_HaltChannel(PAUSE_MUSIC_CHANNEL);
        Mix_ResumeMusic();
    }  
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

void Audio::volumeUp()
{
    _volume += _volume == MAX_VOLUME ? 0 : 10;
    Mix_VolumeMusic(_volume);
    Mix_Volume(PAUSE_MUSIC_CHANNEL, _volume);
}

void Audio::volumeDown()
{
    _volume -= _volume == MIN_VOLUME ? 0 : 10;
    Mix_VolumeMusic(_volume);
    Mix_Volume(PAUSE_MUSIC_CHANNEL, _volume);
}