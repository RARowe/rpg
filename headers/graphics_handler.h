#ifndef GRAPHICS_HANDLER_H
#define GRAPHICS_HANDLER_H
#include <SDL2/SDL.h>
#include "entity.h"
class Entity;

class GraphicsHandler
{
    public:
        virtual void update(Entity& e, const float timeStep) = 0;
};
#endif
