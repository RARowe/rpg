#include "levels.h"

static LevelData LONELY_TOWN =
{
    "lonely_town",
    std::vector<Scenes>
    {
        Scenes::LONELY_TOWN_ENTRANCE,
        Scenes::LONELY_TOWN_OUTSKIRTS,
        Scenes::LONELY_TOWN_OUTSKIRTS_BUILDING
    }
};

const LevelData& getLevelData(Levels level)
{
    return LONELY_TOWN;
}