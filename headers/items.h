#ifndef ITEMS_H
#define ITEMS_H
#include <string>
#include "enums.h"

typedef struct Item
{
    ItemType type;
    ItemSheetTexture texture;
    std::string description;
} Item;

typedef struct InventoryItem
{
    int count;
    Item item;
} InventoryItem;

Item getItem(ItemType t);
#endif