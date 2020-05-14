#pragma once
#ifndef STATIC_ITEM_GRAPHICS_FACTORY_H
#define STATIC_ITEM_GRAPHICS_FACTORY_H
#include <functional>
#include "enums.h"
#include "graphics_context.h"
#include "graphics_handler.h"
#include "static_item_graphics.h"

static std::function<void (GraphicsContext&, Entity&, const float)> drawFromFile(std::string filePath)
{
    return [filePath](GraphicsContext& c, Entity& e, const float timeStep) {
        c.drawTexture(e, filePath);
    };
}

static std::function<void (GraphicsContext&, Entity&, const float)> drawFromSpriteSheet(SpriteSheetTexture t)
{
    return [t](GraphicsContext& c, Entity& e, const float timeStep) {
        c.drawTile(TileSets::OUTDOOR, (int)t, e.getX(), e.getY(), e.getW(), e.getH());
    };
}

static std::function<void (GraphicsContext&, Entity&, const float)> drawFromItemSheet(ItemSheetTexture t)
{
    return [t](GraphicsContext& c, Entity& e, const float timeStep) {
        c.drawTile(TileSets::OUTDOOR, (int)t, e.getX(), e.getY(), e.getW(), e.getH());
    };
}

class StaticItemGraphicsFactory
{
    public:
        static GraphicsHandler* getGraphics(GraphicsContext* context, EntityType type)
        {
            switch (type)
            {
                case EntityType::TRASH:
                    static StaticItemGraphics trash(context, drawFromSpriteSheet(SpriteSheetTexture::TRASH));
                    return &trash;
                case EntityType::LONELY_TOWN_SIGN:
                    static StaticItemGraphics sign(context, drawFromFile("lonely_town_sign.png"));
                    return &sign;
                case EntityType::CD:
                    static StaticItemGraphics cd(context, drawFromItemSheet(ItemSheetTexture::CD));
                    return &cd;
                case EntityType::WARP_POINT:
                    static StaticItemGraphics door(context, drawFromSpriteSheet(SpriteSheetTexture::WOODEN_DOOR_ROUNDED_WINDOW_CLOSED));
                    return &door;
                default:
                    return nullptr;
            }
        }
};
#endif