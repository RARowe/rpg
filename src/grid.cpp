#include <stdio.h>
#include <string>
#include "backgrounds.h"
#include "grid.h"
#include "types.h"

static inline void setSource(const char* tileCode, Point& pointCode)
{
    int xCode = 0;
    int yCode = 0;

    sscanf(tileCode, "%c%d", (char*)&yCode, &xCode);

    yCode -= 65; // set A = 0, B = 1, etc

    pointCode.x = xCode * 17; // 16x16 tiles with 1 pixel padding
    pointCode.y = yCode * 17;
}

Grid::Grid(const GraphicsContext& context, BackgroundCache* cache)
{
    _tiles = context.getTexture("tiles.png");
	_cache = cache;
}

Grid::~Grid()
{
    SDL_DestroyTexture(_tiles);
}

void Grid::load(const std::string& backgroundKey)
{
	// The following user pointer arithmetic. This should probably be revised
	const char* backgroundString = _cache->getBackground(backgroundKey).c_str();
	int offset = 0;
    for (int i = 0; i < ROWS * COLUMNS; i++)
    {
        setSource(backgroundString + offset, _tileCodes[i]);
		while (backgroundString[++offset] != ' '); // Find next space
		offset += 1; // Start at first character
    }
}

void Grid::draw(SDL_Renderer* renderer)
{
    SDL_Rect src = {0,0,16,16};
    SDL_Rect out = {0,0,32,32};
    Point p;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            p = _tileCodes[(COLUMNS * i) + j];
            out.x = j * 32;
            out.y = i * 32;
            src.x = p.x;
            src.y = p.y;
            SDL_RenderCopy(renderer, _tiles, &src, &out);
        }
    }
}
