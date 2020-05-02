#include "game_context.h"
#include "scenes.h"

static SceneData LONELY_TOWN_OUTSKIRTS =
{
    "resources/backgrounds/lonely_town/outskirts",
    std::vector<EntityType> { EntityType::NEWSPAPER_RACK },
    std::vector<InteractData>
    {
        {
            3,
            1,
            [](GameContext& c)
            {
                if (!c.gameEventHasHappened(GameEvent::FOUND_ENCRYPTED_COMPACT_DISK))
                {
                    c.getPlayer()->addItem("encrypted compact disk");
                    c.openTextBox(TileSets::ITEMS, (int)ItemSheetTexture::CD, "Found 'Encrypted compact disk'.");
                    c.getAudio().playSound("audio/found_item.ogg");
                    c.broadcastGameEvent(GameEvent::FOUND_ENCRYPTED_COMPACT_DISK);
                }
                else
                {
                    c.openDialog("tim.png", "It's a newspaper rack.");
                }
            }
        }
    },
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
    std::vector<InteractData>
    {
        {
            1,
            12,
            [](GameContext& c)
            {
                if (!c.gameEventHasHappened(GameEvent::FOUND_CABINET_KEY))
                {
                    c.getPlayer()->addItem("cabinet key");
                    c.openTextBox(TileSets::ITEMS, (int)ItemSheetTexture::CABINET_KEY, "Found 'Cabinet key'.");
                    c.getAudio().playSound("audio/found_item.ogg");
                    c.broadcastGameEvent(GameEvent::FOUND_CABINET_KEY);
                }
            }
        }
    },
    std::vector<WarpPointData> { },
    TileSets::OUTDOOR
};

static SceneData LONELY_TOWN_OUTSKIRTS_BUILDING =
{
    "resources/backgrounds/lonely_town/outskirts_building",
    std::vector<EntityType> { },
    std::vector<InteractData>
    {
        {
            3,
            9,
            [](GameContext& c) {
                if (c.gameEventHasHappened(GameEvent::FOUND_CABINET_KEY))
                {
                    if (c.gameEventHasHappened(GameEvent::LONELY_TOWN_OUTSKIRTS_BUILDING_ITEM_FOUND))
                    {
                        c.openDialog("tim.png", "It's an empty cabinet.");
                    }
                    else
                    {
                        c.getPlayer()->addItem("bass skrangs");
                        c.openTextBox(TileSets::ITEMS, (int)ItemSheetTexture::STRINGS, "Found 'Bass strings'.");
                        c.getAudio().playSound("audio/found_item.ogg");
                        c.broadcastGameEvent(GameEvent::LONELY_TOWN_OUTSKIRTS_BUILDING_ITEM_FOUND);
                    }
                }
                else
                {
                    c.openDialog("tim.png", "It's locked... I wonder what's inside... Then again, this isn't my house!");
                }
            }
        }
    },
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