#ifndef INTERACT_HANDLER_H
#define INTERACT_HANDLER_H
#include "entity.h"
#include "enums.h"
#include "event_handler.h"
#include "game_context.h"

class InteractHandler : public EventHandler
{
    public:
        InteractHandler(GameContext* context, const char* imagePath, const char* text)
        {
            _context = context;
            _imagePath = imagePath;
            _text = text;
        }
        void update(Entity& e, EventType event, Entity& src)
        {
            if (event == EventType::INTERACT && e.pointInside(src.getCursor()))
            {
                _context->openDialog(_imagePath, _text);
            }
        }
    private:
        GameContext* _context;
        const char* _imagePath;
        const char* _text;
};
#endif
