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

const Item& getItem(ItemType t);
#endif