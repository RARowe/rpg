#include <stdio.h>
#include "backgrounds.h"
#include "grid.h"
#include "types.h"

Grid::Grid(GraphicsContext& context)
{
    _tiles = context.getTexture("tiles.png");
}

Grid::~Grid()
{
    SDL_DestroyTexture(_tiles);
}

static inline void setSource(const char* tileCode, SDL_Rect& src)
{
    int xCode = 0;
    int yCode = 0;
    sscanf(tileCode, "%c%d", (char*)&yCode, &xCode);

    yCode -= 65; // set A = 0, B = 1, etc

    src.x = xCode * 17; // 16x16 tiles with 1 pixel padding
    src.y = yCode * 17;
}

void Grid::draw(SDL_Renderer* renderer)
{
    SDL_Rect src = {0,0,16,16};
    SDL_Rect out = {0,0,32,32};
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            setSource(FIELD[(COLUMNS * i) + j], src);
            out.x = j * 32;
            out.y = i * 32;
            SDL_RenderCopy(renderer, _tiles, &src, &out);
        }
    }
}
