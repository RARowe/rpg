#include "scenes.h"

static SceneData LONELY_TOWN_OUTSKIRTS =
{
    "resources/backgrounds/lonely_town/outskirts",
    std::vector<EntityType> { EntityType::NEWSPAPER_RACK, EntityType::WARP_POINT },
    TileSets::OUTDOOR
};

static SceneData LONELY_TOWN_ENTRANCE =
{
    "resources/backgrounds/lonely_town/entrance",
    std::vector<EntityType> { EntityType::LONELY_TOWN_SIGN, EntityType::TRASH, EntityType::BUCKET_HEAD },
    TileSets::OUTDOOR
};

static SceneData LONELY_TOWN_OUTSKIRTS_BUILDING =
{
    "resources/backgrounds/lonely_town/outskirts_building",
    std::vector<EntityType> { },
    TileSets::INDOOR
};

const SceneData& getSceneData(Scenes s)
{
    switch (s)
    {
        case Scenes::LONELY_TOWN_OUTSKIRTS:
            return LONELY_TOWN_OUTSKIRTS;
        case Scenes::LONELY_TOWN_ENTRANCE:
            return LONELY_TOWN_ENTRANCE;
        case Scenes::LONELY_TOWN_OUTSKIRTS_BUILDING:
            return LONELY_TOWN_OUTSKIRTS_BUILDING;
        default:
            return LONELY_TOWN_OUTSKIRTS;
    }
}