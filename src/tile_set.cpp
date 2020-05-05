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

static TileSetData ITEMS =
{
    16,
    16,
    1,
    1,
    3,
    "tile_sets/items.png"
};

static TileSetData MISC =
{
    16,
    16,
    1,
    1,
    1,
    "tile_sets/misc.png"
};

const TileSetData& getTileSetData(TileSets t)
{
    switch (t)
    {
        case TileSets::OUTDOOR:
            return OUTDOOR;
        case TileSets::INDOOR:
            return INDOOR;
        case TileSets::ITEMS:
            return ITEMS;
        case TileSets::MISC:
            return MISC;
        default:
            return OUTDOOR;
    }
}