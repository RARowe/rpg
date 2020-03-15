#ifndef GRID_H
#define GRID_H 
#include <SDL2/SDL.h>
#include "drawable.h"
#include "interactable.h"
#include "graphics_context.h"
#include "tile.h"
#include "types.h"

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
        Point _tileCodes[ROWS * COLUMNS];
};
#endif
