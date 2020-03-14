#ifndef TILE_H
#define TILE_H 
#include <SDL2/SDL.h>
#include "drawable.h"
class Tile : public Drawable
{
    public:
        Tile(SDL_Texture* texture, int x, int y);
        ~Tile();
        void draw(SDL_Renderer* renderer);
    private:
        SDL_Rect _rect;
        SDL_Texture* _texture;
};
#endif
