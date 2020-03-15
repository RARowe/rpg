#include <SDL2/SDL.h>
#include "drawable.h"
#include "interactable.h"
#include "graphics_context.h"
#include "tile.h"

#ifndef GRID_H
#define GRID_H 
#define COLUMNS 19
#define ROWS 13
class Grid : public Drawable
{
    public:
        Grid(GraphicsContext& context);
        ~Grid();
        void addEntity(SDL_Texture* entity, int column, int row);
        void draw(SDL_Renderer* renderer);
    private:
        SDL_Texture* _tiles;
};
#endif
