#pragma once
#ifndef TRASH_GRAPHICS_H
#define TRASH_GRAPHICS_H
#include <string>
#include "entity.h"
#include "graphics_context.h"
#include "graphics_handler.h"
#include "static_item_graphics.h"
class TrashGraphics : public GraphicsHandler
{
    public:
        static TrashGraphics* getInstance(GraphicsContext* context)
        {
            static TrashGraphics graphics(context);
            return &graphics;
        }
        void update(Entity& e, const TimeStep timeStep)
        {
            StaticItemGraphics graphics(_context, _trashPath);
            graphics.update(e, timeStep);
        }
    private:
        TrashGraphics(GraphicsContext* context) {}
        GraphicsContext* _context;
        static const std::string _trashPath("trash.png");
};
#endif