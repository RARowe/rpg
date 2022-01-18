#include "platform.h"

#include <SDL2/SDL_mixer.h>

#include <stdio.h>

/* Internal Declarations */
typedef struct {
    int size;
} SoundQueue;


static int audio_init(Audio* a);
static void audio_process(Audio* a);
static void audio_shutdown(Audio* a);

/* SDL Audio Platform Implementation */
typedef struct Audio {
    Mix_Music* music;
    Mix_Chunk* sounds;
    bool stopRequested;
    SoundQueue queue;
} Audio;

void
audio_queue_sound(Audio* a, int soundId) {
    // TODO: This does nothing interesting
    a->queue.size += 1;
}

void
audio_request_stop_music(Audio* a) {
    a->stopRequested = true;
}

/* Internal Definitions */
static int
audio_init(Audio* a) {
    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        printf("SDL_Mixer: Unable to open mixer. Error: %s\n", Mix_GetError());
        return 0;
    }

    // TODO: These should go somewhere else
    a->music = Mix_LoadMUS("resources/welcome.wav");
    a->sounds = Mix_LoadWAV("resources/start.ogg");
    a->stopRequested = false;
    a->queue.size = 0;

    Mix_PlayMusic(a->music, -1);

    return 1;
}

static void
audio_process(Audio* a) {
    if (a->stopRequested) {
        Mix_HaltMusic();
    }

    for (int i = 0; i < a->queue.size; i++) {
        Mix_PlayChannel(-1, a->sounds, 0);
    }
}

static void
audio_shutdown(Audio* a) {
    Mix_FreeChunk(a->sounds);
    Mix_FreeMusic(a->music);
    Mix_CloseAudio();
}

