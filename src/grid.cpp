#include "grid.h"

Grid::Grid(GraphicsContext& context)
{
    SDL_Texture* texture = context.getTexture("grass1.png");
    for (int i = 0; i < 38; i++)
    {
        for (int j = 0; j < 25; j++)
        {
            _tiles[i][j] = new Tile(texture, i * 16, j * 16, false);
        }
    }
}

Grid::~Grid()
{
    for (int i = 0; i < 38; i++)
    {
        for (int j = 0; j < 25; j++)
        {
            delete _tiles[i][j];
        }
    }
}

void Grid::draw(SDL_Renderer* renderer)
{
    for (int i = 0; i < 38; i++)
    {
        for (int j = 0; j < 25; j++)
        {
            _tiles[i][j]->draw(renderer);
        }
    }
}

void Grid::addEntity(SDL_Texture* entity, int column, int row)
{
    _tiles[column][row]->addEntity(entity);
}

bool Grid::isCollision(int x, int y)
{
    return _tiles[x / 16][y / 16]->isBlocking();
}
