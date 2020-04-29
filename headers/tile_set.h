#ifndef TILE_SET_H
#define TILE_SET_H
#include <string>
#include "enums.h"

typedef struct TileSetData
{
    int tileWidth;
    int tileHeight;
    int margin;
    int rows;
    int columns;
    const std::string path;
} TileSetData;

const TileSetData& getTileSetData(TileSets t);
#endif