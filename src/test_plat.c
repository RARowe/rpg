#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_HEIGHT 416
#define SCREEN_WIDTH 608
#define DEBUG puts

#include "platform_audio.c"
#include "platform_input.c"
#include "platform_graphics.c"
