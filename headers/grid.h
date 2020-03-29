#ifndef GRID_H
#define GRID_H 
#include <SDL2/SDL.h>
#include "background_cache.h"
#include "graphics_context.h"
#include "types.h"

#define COLUMNS 19
#define ROWS 13
class Grid
{
    public:
        Grid(const GraphicsContext& context, BackgroundCache* cache);
        ~Grid();
        void load(const std::string& backgroundKey);
        void draw(SDL_Renderer* renderer);
    private:
        BackgroundCache* _cache;
        SDL_Texture* _tiles;
        Point _tileCodes[ROWS * COLUMNS];
};
#endif
