#include <vector>
#include "items.h"

static std::vector<Item> ITEMS =
{
    { ItemType::ENCRYPTED_CD, ItemSheetTexture::CD, "Encrypted CD" },
    { ItemType::CABINET_KEY, ItemSheetTexture::KEY, "Cabinet Key" },
    { ItemType::BASS_STRINGS, ItemSheetTexture::STRINGS, "Bass strings" }
};

const Item& getItem(ItemType type) {
    for (auto i : ITEMS)
    {
        if (i.type == type)
        {
            return i;
        }
    }

    return ITEMS[0];
}