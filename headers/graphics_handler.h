#ifndef GRAPHICS_HANDLER_H
#define GRAPHICS_HANDLER_H
#include <SDL2/SDL.h>
#include "entity.h"
#include "time_step.h"
class Entity;

class GraphicsHandler
{
    public:
        virtual void update(Entity& e, const TimeStep timeStep) = 0;
};
#endif
