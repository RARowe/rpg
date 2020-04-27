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

PlayerGraphics::PlayerGraphics(GraphicsContext* context) : _context(context)
{
}

void PlayerGraphics::update(Entity& e, const float timeStep)
{
    float newTime = _ticks + timeStep;
    _ticks = newTime > 1.0f ? 0.0f : newTime;
    std::string* textureName;
    switch (e.getDirection())
    {
        case Direction::UP:
            textureName = _up;
            break;
        case Direction::DOWN:
            textureName = _down;
            break;
        case Direction::LEFT:
            textureName = _left;
            break;
        case Direction::RIGHT:
            textureName = _right;
            break;
        default:
            break;
    }
    std::string& still = textureName[0];
    std::string& walk1 = textureName[1];
    std::string& walk2 = textureName[2];

    if (e.isMoving())
    {
        float frame = std::abs(_ticks - 0.5f);
        if (frame < 0.25f)
        {
            _context->drawTexture(e.getX(), e.getY() - 17, e.getW(), e.getH() + 17, walk1);
        }
        else
        {
            _context->drawTexture(e.getX(), e.getY() - 17, e.getW(), e.getH() + 17, walk2);
        }
    }
    else
    {
        _context->drawTexture(e.getX(), e.getY() - 17, e.getW(), e.getH() + 17, still);
    }

    if (e.getState() == (int)PlayerStateType::ITEM_FOUND)
    {
        _context->drawAbove(e, "items.png", (int)ItemSheetTexture::CD);
    }
    _context->drawHitbox(e.getX(), e.getY(), e.getW(), e.getH());
}
