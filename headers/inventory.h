#ifndef INVENTORY_H
#define INVENTORY_H
#include "enums.h"
#include "types.h"
bool inventory_add_item(Inventory* inv, ItemType type);
bool inventory_take_item(Inventory* inv, ItemType type);
// TODO: This may fit in a better spot
const char* inventory_get_item_description(ItemType type);
// TODO: Remove this
ItemSheetTexture inventory_get_item_texture(ItemType type);
#endif
