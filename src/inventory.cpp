#include "inventory.h"
#include "types.h"

// TODO: This type of configuration should not live in code
static const char* ENCRYPTED_CD_DESC = "Encrypted CD";
static const char* CABINET_KEY_DESC = "Cabinet Key";
static const char* BASS_STRINGS_DESC = "Bass Strings";
static const char* UNKNOWN_DESC = "UNKNOWN";

// TODO: Max size of inventory should not be determined here
static const uint8_t MAX_INV_SIZE = 112; // 14 columns by 8 rows

// TODO: Item list always adds to end, there could be better algo
//       it may be good to have "empty" item type to have "holes"
//       in inventory
bool inventory_add_item(Inventory* inv, ItemType type) {
    uint8_t size = inv->size;
    if (size < MAX_INV_SIZE) {
        inv->items[size] = type;
        inv->mostRecentlyAdded = type;
        inv->size++;
        return true;
    }

    return false;
}

// Get item, if available, and shifts items down to fill place
bool inventory_take_item(Inventory* inv, ItemType type) {
    ItemType* items = inv->items;
    uint8_t size = inv->size;
    bool hasItem = false;
    uint8_t shiftItemIndex = -1;
    for (uint8_t i = 0; i < size; i++) {
        if (hasItem) {
            items[shiftItemIndex] = items[i];
            shiftItemIndex++;
            continue;
        }

        if (items[i] == type) {
            hasItem = true;
            shiftItemIndex = i;
            inv->size--;
        }
    }

    return hasItem;
}

const char* inventory_get_item_description(ItemType type) {
    switch (type) {
        case ItemType::ENCRYPTED_CD: return ENCRYPTED_CD_DESC;
        case ItemType::CABINET_KEY: return CABINET_KEY_DESC;
        case ItemType::BASS_STRINGS: return BASS_STRINGS_DESC;
        default: return UNKNOWN_DESC;
    }
}

ItemSheetTexture inventory_get_item_texture(ItemType type) {
    switch (type) {
        case ItemType::ENCRYPTED_CD: return ItemSheetTexture::CD;
        case ItemType::CABINET_KEY: return ItemSheetTexture::KEY;
        case ItemType::BASS_STRINGS: return ItemSheetTexture::STRINGS;
        default: return ItemSheetTexture::KEY;
    }
}
