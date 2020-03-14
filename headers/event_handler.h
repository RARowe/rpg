#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H
#include "entity.h"
#include "enums.h"
class Entity;

class EventHandler
{
    public:
        virtual void update(Entity& e, EventType event, Entity& src) = 0;
};
#endif
