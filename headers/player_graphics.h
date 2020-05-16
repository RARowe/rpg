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
        void update(Entity& e, const TimeStep timeStep);
    private:
        PlayerGraphics(GraphicsContext* context);
        GraphicsContext* _context;
};
#endif
