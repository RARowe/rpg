#include "tile_set.h"

static TileSetData INDOOR =
{
    16,
    16,
    1,
    19,
    27,
    "tile_sets/indoors.png"
};

static TileSetData OUTDOOR =
{
    16,
    16,
    1,
    28,
    37,
    "tile_sets/outdoors.png"
};

const TileSetData& getTileSetData(TileSets t)
{
    switch (t)
    {
        case TileSets::OUTDOOR:
            return OUTDOOR;
        case TileSets::INDOOR:
            return INDOOR;
        default:
            return OUTDOOR;
    }
}