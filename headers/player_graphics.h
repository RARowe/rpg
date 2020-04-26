#pragma once
#ifndef PLAYER_GRAPHICS_H
#define PLAYER_GRAPHICS_H
#include <string>
#include "graphics_context.h"
#include "graphics_handler.h"

class PlayerGraphics : public GraphicsHandler
{
    public:
        static PlayerGraphics* getInstance(GraphicsContext* context);
        void update(Entity& e, const float timeStep);
    private:
        PlayerGraphics(GraphicsContext* context);
        GraphicsContext* _context;
        float _ticks = 0.0f;
        std::string _left[3] = { "l.png", "wl1.png", "wl2.png" };
        std::string _right[3] = { "r.png", "wr1.png", "wr2.png" };
        std::string _up[3] = { "u.png", "wu1.png", "wu2.png" };
        std::string _down[3] = { "d.png", "wd1.png", "wd2.png" };
};
#endif
