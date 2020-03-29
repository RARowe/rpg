#ifndef INTERACT_HANDLER_H
#define INTERACT_HANDLER_H
#include "entity.h"
#include "enums.h"
#include "event_handler.h"
#include "game_context.h"

class InteractHandler : public EventHandler
{
    public:
        void update(Entity& e, EventType event, Entity& src)
        {
            if (event == EventType::INTERACT && e.pointInside(src.getCursor()))
            {
                handleInteractEvent(e, src);
            }
        }
    protected:
        virtual void handleInteractEvent(Entity& e, Entity& src) = 0;
};
#endif
