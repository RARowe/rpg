#ifndef STATIC_ITEM_GRAPHICS_H
#define STATIC_ITEM_GRAPHICS_H 
#include <SDL2/SDL.h>
#include "entity.h"
#include "enums.h"
#include "graphics_context.h"
#include "graphics_handler.h"

class StaticItemGraphics : public GraphicsHandler
{
    public:
        StaticItemGraphics(GraphicsContext* context, std::string name);
        void update(Entity& e, const float timeStep);
    private:
		GraphicsContext* _context;
        std::string _name;
};
#endif
