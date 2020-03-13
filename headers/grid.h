#include <SDL2/SDL.h>
#include "drawable.h"
#include "interactable.h"
#include "collidable.h"
#include "graphics_context.h"
#include "tile.h"

#ifndef GRID_H
#define GRID_H 
class Grid : public Drawable, public Collidable
{
    public:
        Grid(GraphicsContext& context);
        ~Grid();
        void addEntity(SDL_Texture* entity, int column, int row);
        void draw(SDL_Renderer* renderer);
        bool isCollision(int x, int y);
    private:
        Tile* _tiles[38][25];
};
#endif
