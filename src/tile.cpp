#include <SDL2/SDL.h>
#include "drawable.h"
#include "tile.h"

Tile::Tile(SDL_Texture* texture, int x, int y)
{
    _texture = texture;
    _rect.x = x;
    _rect.y = y;
    _rect.h = 32;
    _rect.w = 32;
}

Tile::~Tile()
{
    SDL_DestroyTexture(_texture);
}

void Tile::draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, _texture, NULL, &_rect);
}
