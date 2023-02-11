#pragma once
#ifndef GAME_H
#define GAME_H
#include "platform.h"
#include <stddef.h>


typedef struct GameData GameData;

size_t game_size_of_game_data();
int game_init(GameData *data);
void game_run_frame(GameData *d, Graphics *g, Audio *a, Input *i,
                    float timeStep);
#endif
