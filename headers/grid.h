#ifndef GRID_H
#define GRID_H 
#include <SDL2/SDL.h>
#include "graphics_context.h"
#include "types.h"

#define COLUMNS 19
#define ROWS 13
class Grid
{
    public:
        Grid(const GraphicsContext& context);
        ~Grid();
        void load(const char background[][4]);
        void draw(SDL_Renderer* renderer);
    private:
        SDL_Texture* _tiles;
        Point _tileCodes[ROWS * COLUMNS];
};
#endif
