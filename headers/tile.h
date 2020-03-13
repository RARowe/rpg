#include <SDL2/SDL.h>
#include "drawable.h"
#include "entity.h"

#ifndef TILE_H
#define TILE_H 
class Tile : public Drawable
{
    public:
        Tile(SDL_Texture* texture, int x, int y, bool isBlocking);
        ~Tile();
        void draw(SDL_Renderer* renderer);
        bool isBlocking();
        void addEntity(SDL_Texture* entity);
    private:
        SDL_Rect _rect;
        SDL_Texture* _texture;
        SDL_Texture* _entity;
        bool _isBlocking;
};
#endif
