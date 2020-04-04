#pragma once
#ifndef PLAYER_GRAPHICS_H
#define PLAYER_GRAPHICS_H
#include <string>
#include "graphics_context.h"
#include "graphics_handler.h"

class PlayerGraphics : public GraphicsHandler
{
    public:
        PlayerGraphics(GraphicsContext* context);
        void update(Entity& e, SDL_Renderer* renderer);
    private:
        GraphicsContext* _context;
        int _ticks = 0;
        std::string _left[3] = { "l.png", "wl1.png", "wl2.png" };
        std::string _right[3] = { "r.png", "wr1.png", "wr2.png" };
        std::string _up[3] = { "u.png", "wu1.png", "wu2.png" };
        std::string _down[3] = { "d.png", "wd1.png", "wd2.png" };
};
#endif
