#include "scenes.h"

static SceneData LONELY_TOWN_OUTSKIRTS =
{
    "resources/backgrounds/lonely_town/outskirts",
    std::vector<EntityType> { EntityType::NEWSPAPER_RACK },
    std::vector<WarpPointData>
    {
        {
            5,
            13,
            9,
            9,
            Scenes::LONELY_TOWN_OUTSKIRTS_BUILDING,
            "audio/door.ogg"
        }
    },
    TileSets::OUTDOOR
};

static SceneData LONELY_TOWN_ENTRANCE =
{
    "resources/backgrounds/lonely_town/entrance",
    std::vector<EntityType> { EntityType::LONELY_TOWN_SIGN, EntityType::TRASH, EntityType::BUCKET_HEAD },
    std::vector<WarpPointData> { },
    TileSets::OUTDOOR
};

static SceneData LONELY_TOWN_OUTSKIRTS_BUILDING =
{
    "resources/backgrounds/lonely_town/outskirts_building",
    std::vector<EntityType> { },
    std::vector<WarpPointData>
    {
        {
            10,
            9,
            6,
            13,
            Scenes::LONELY_TOWN_OUTSKIRTS,
            "audio/door.ogg"
        }
    },
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