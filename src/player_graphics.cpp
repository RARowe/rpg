#include <SDL2/SDL.h>
#include "entity.h"
#include "graphics_context.h"
#include "player_graphics.h"

PlayerGraphics::PlayerGraphics(GraphicsContext* context)
{
    _context = context;
}

void PlayerGraphics::update(Entity& e,  SDL_Renderer* renderer)
{
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
        int frame = _ticks % GraphicsContext::FRAME_RATE;
        if (frame < 15 || (frame > 30 && frame < 45))
        {
            _context->drawTexture(e, walk1);
        }
        else
        {
            _context->drawTexture(e, walk2);
        }
    }
    else
    {
        _context->drawTexture(e, still);
    }
    _ticks++;
}
