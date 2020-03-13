#include "entity.h"

#ifndef MOVEMENT_HANDLER_H
#define MOVEMENT_HANDLER_H
class MovementHandler
{
    public:
        virtual void update(Entity& e) = 0;
};
#endif
