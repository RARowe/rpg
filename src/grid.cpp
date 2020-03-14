#include "grid.h"

Grid::Grid(GraphicsContext& context)
{
    SDL_Texture* texture = context.getTexture("grass1.png");
    for (int i = 0; i < COLUMNS; i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            _tiles[i][j] = new Tile(texture, i * 32, j * 32);
        }
    }
}

Grid::~Grid()
{
    for (int i = 0; i < COLUMNS; i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            delete _tiles[i][j];
        }
    }
}

void Grid::draw(SDL_Renderer* renderer)
{
    for (int i = 0; i < COLUMNS; i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            _tiles[i][j]->draw(renderer);
        }
    }
}
