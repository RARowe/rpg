#ifndef INTERACT_HANDLER_H
#define INTERACT_HANDLER_H
#include "entity.h"
#include "enums.h"
#include "event_handler.h"
#include "game_context.h"

class InteractHandler : public EventHandler
{
    public:
        InteractHandler(GameContext* context)
        {
            _context = context;
        }
        void update(Entity& e, EventType event, Entity& src)
        {
            if (event == EventType::INTERACT && e.pointInside(src.getCursor()))
            {
                _context->openDialog(NULL, NULL);
            }
        }
    private:
        GameContext* _context;
};
#endif
