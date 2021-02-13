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
    int x = e.pos.x, y = e.pos.y, w = e.body.w, h = e.body.h;
    Direction d = e.direction;
    if (e.isMoving())
    {
        _context->drawWalkingSprite(timeStep, d, "theo", e);
    }
    else
    {
        _context->drawStandingSprite(d, "theo", e);
    }

    if (e.state == (int)PlayerStateType::ITEM_FOUND)
    {
        _context->drawAbove(e, TileSets::ITEMS, (int)e.getMostRecentlyAddedItem().texture);
    }
    _context->drawHitbox(x, y, w, h);
}
