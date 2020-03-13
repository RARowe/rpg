#include <SDL2/SDL.h>
#include "entity.h"
#include "graphics_context.h"
#include "player_graphics.h"

PlayerGraphics::PlayerGraphics(GraphicsContext& context)
{
    _down[0] = context.getTexture("d.png");
    _down[1] = context.getTexture("wd1.png");
    _down[2] = context.getTexture("wd2.png");
    _up[0] = context.getTexture("u.png");
    _up[1] = context.getTexture("wu1.png");
    _up[2] = context.getTexture("wu2.png");
    _left[0] = context.getTexture("l.png");
    _left[1] = context.getTexture("wl1.png");
    _left[2] = context.getTexture("wl2.png");
    _right[0] = context.getTexture("r.png");
    _right[1] = context.getTexture("wr1.png");
    _right[2] = context.getTexture("wr2.png");
	_ticks = 0;
}

PlayerGraphics::~PlayerGraphics()
{
    for (int i = 0; i < 3; i++)
    {
        SDL_DestroyTexture(_down[i]);
    }
    for (int i = 0; i < 3; i++)
    {
        SDL_DestroyTexture(_up[i]);
    }
    for (int i = 0; i < 3; i++)
    {
        SDL_DestroyTexture(_left[i]);
    }
    for (int i = 0; i < 3; i++)
    {
        SDL_DestroyTexture(_right[i]);
    }
}

void PlayerGraphics::update(Entity& e, SDL_Renderer* renderer)
{
    SDL_Texture** textures;
    switch (e.getDirection())
    {
        case Direction::UP:
            textures = _up;
            break;
        case Direction::DOWN:
            textures = _down;
            break;
        case Direction::LEFT:
            textures = _left;
            break;
        case Direction::RIGHT:
            textures = _right;
            break;
        default:
            break;
    }
    SDL_Texture* texture;
    SDL_Texture* still = textures[0];
    SDL_Texture* walk1 = textures[1];
    SDL_Texture* walk2 = textures[2];

    if (e.isMoving())
    {
        int frame = _ticks % GraphicsContext::FRAME_RATE;
        if (frame < 15 || (frame > 30 && frame < 45))
        {
            texture = walk1;
        }
        else
        {
            texture = walk2;
        }
    }
    else
    {
        texture = still;
    }
    _rect.x = e.getX();
    _rect.y = e.getY();
	_rect.h = e.getH();
	_rect.w = e.getW();
    SDL_RenderCopy(renderer, texture, NULL, &_rect);
    _ticks++;
}
