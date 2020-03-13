#include "static_item_graphics.h"

StaticItemGraphics::StaticItemGraphics(SDL_Texture* texture) : _texture(texture)
{
}

StaticItemGraphics::~StaticItemGraphics()
{
	SDL_DestroyTexture(_texture);
}

void StaticItemGraphics::update(Entity& e, SDL_Renderer* renderer)
{
	_rect.x = e.getX();
	_rect.y = e.getY();
	_rect.h = e.getH();
	_rect.w = e.getW();
    SDL_RenderCopy(renderer, _texture, NULL, &_rect);
}