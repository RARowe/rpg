#include <SDL2/SDL.h>
#include "drawable.h"
#include "tile.h"

Tile::Tile(SDL_Texture* texture, int x, int y, bool isBlocking)
{
    _texture = texture;
    _rect.x = x;
    _rect.y = y;
    _rect.h = 16;
    _rect.w = 16;
    _isBlocking = isBlocking;
    _entity = NULL;
}

Tile::~Tile()
{
    SDL_DestroyTexture(_texture);
}

void Tile::draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, _texture, NULL, &_rect);
    if (_entity)
    {
        SDL_RenderCopy(renderer, _entity, NULL, &_rect);
    }
}

bool Tile::isBlocking()
{
    return _isBlocking;
}

void Tile::addEntity(SDL_Texture* entity)
{
    _entity = entity;
    _isBlocking = true;
}
