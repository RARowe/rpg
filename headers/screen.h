#pragma once
#ifndef SCREEN_H
#define SCREEN_H
#include <SDL2/SDL.h>
#include "entity.h"
#include "grid.h"

class Screen
{
    public:
        Screen();
        ~Screen();
        void draw(const SDL_Renderer* renderer);
    private:
        Grid* _grid;
};
#endif
