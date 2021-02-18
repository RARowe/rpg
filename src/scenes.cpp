#include "game_context.h"
#include "inventory.h"
#include "scenes.h"

static SceneData LONELY_TOWN_OUTSKIRTS =
{
    std::vector<EntityType> { },
    std::vector<InteractData>
    {
        {
            3,
            1,
            [](GameContext& c)
            {
                if (!c.gameEventHasHappened(GameEvent::FOUND_ENCRYPTED_COMPACT_DISK))
                {
                    inventory_add_item(c.inventory, ItemType::ENCRYPTED_CD);
                    c.openTextBox(TileSets::ITEMS, (int)ItemSheetTexture::CD, "Found 'Encrypted compact disk'.");
                    c.audio.playSound("audio/found_item.ogg");
                    c.broadcastGameEvent(GameEvent::FOUND_ENCRYPTED_COMPACT_DISK);
                }
                else
                {
                    c.openDialog("tim.png", "It's a newspaper rack.");
                }
            }
        }
    },
    TileSets::OUTDOOR,
    1
};

static SceneData LONELY_TOWN_ENTRANCE =
{
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
                    inventory_add_item(c.inventory, ItemType::CABINET_KEY);
                    c.openTextBox(TileSets::ITEMS, (int)ItemSheetTexture::KEY, "Found 'Cabinet key'.");
                    c.audio.playSound("audio/found_item.ogg");
                    c.broadcastGameEvent(GameEvent::FOUND_CABINET_KEY);
                }
            }
        }
    },
    TileSets::OUTDOOR,
    0
};

static SceneData LONELY_TOWN_OUTSKIRTS_BUILDING =
{
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
                        inventory_add_item(c.inventory, ItemType::BASS_STRINGS);
                        c.openTextBox(TileSets::ITEMS, (int)ItemSheetTexture::STRINGS, "Found 'Bass strings'.");
                        c.audio.playSound("audio/found_item.ogg");
                        c.broadcastGameEvent(GameEvent::LONELY_TOWN_OUTSKIRTS_BUILDING_ITEM_FOUND);
                        inventory_take_item(c.inventory, ItemType::CABINET_KEY);
                    }
                }
                else
                {
                    c.openDialog("tim.png", "It's locked... I wonder what's inside... Then again, this isn't my house!");
                }
            }
        }
    },
    TileSets::INDOOR,
    0
};

static SceneData HOUSE =
{
    std::vector<EntityType> {},
    std::vector<InteractData> {},
    TileSets::INDOOR,
    0
};

SceneData getSceneData(Scenes s)
{
    switch (s)
    {
        case Scenes::LONELY_TOWN_OUTSKIRTS:
            return LONELY_TOWN_OUTSKIRTS;
        case Scenes::LONELY_TOWN_ENTRANCE:
            return LONELY_TOWN_ENTRANCE;
        case Scenes::LONELY_TOWN_OUTSKIRTS_BUILDING:
            return LONELY_TOWN_OUTSKIRTS_BUILDING;
        case Scenes::LONELY_TOWN_HOUSE:
            return HOUSE;
        default:
            return LONELY_TOWN_OUTSKIRTS;
    }
}
