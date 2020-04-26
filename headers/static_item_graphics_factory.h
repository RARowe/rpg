#pragma once
#ifndef STATIC_ITEM_GRAPHICS_FACTORY_H
#define STATIC_ITEM_GRAPHICS_FACTORY_H
#include "enums.h"
#include "graphics_context.h"
#include "graphics_handler.h"
#include "static_item_graphics.h"

class StaticItemGraphicsFactory
{
    public:
        static GraphicsHandler* getGraphics(GraphicsContext* context, EntityType type)
        {
            switch (type)
            {
                case EntityType::TRASH:
                    static StaticItemGraphics trash(context, "trash.png");
                    return &trash;
                case EntityType::LONELY_TOWN_SIGN:
                    static StaticItemGraphics sign(context, "lonely_town_sign.png");
                    return &sign;
                default:
                    return nullptr;
            }
        }
};
#endif