#include "entity.h"

#ifndef MOVEMENT_HANDLER_H
#define MOVEMENT_HANDLER_H
class MovementHandler
{
    public:
        virtual void update(Entity& e, const float timeStep) = 0;
};
#endif
