#include <SDL2/SDL.h>
#include <cmath>
#include "entity.h"
#include "graphics_context.h"
#include "player_graphics.h"

PlayerGraphics* PlayerGraphics::getInstance(GraphicsContext* context)
{
    static PlayerGraphics handler(context);
    return &handler;
}

PlayerGraphics::PlayerGraphics(GraphicsContext* context) : _context(context) { }

void PlayerGraphics::update(Entity& e, const TimeStep timeStep)
{
    int x = e.getX(), y = e.getY(), w = e.getW(), h = e.getH();
    Direction d = e.getDirection();
    if (e.isMoving())
    {
        _context->drawWalkingSprite(timeStep, d, 0, x, y, w, h);
    }
    else
    {
        _context->drawStandingSprite(d, 0, x, y, w, h);
    }

    if (e.getState() == (int)PlayerStateType::ITEM_FOUND)
    {
        _context->drawAbove(e, TileSets::ITEMS, (int)e.getMostRecentlyAddedItem().texture);
    }
    _context->drawHitbox(x, y, w, h);
}
