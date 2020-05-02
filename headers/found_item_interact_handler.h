#ifndef FOUND_ITEM_INTERACT_HANDLER_H
#define FOUND_ITEM_INTERACT_HANDLER_H
#include <functional>
#include "entity.h"
#include "enums.h"
#include "game_context.h"
#include "interact_handler.h"

class FoundItemInteractHandler : public InteractHandler
{
    public:
        FoundItemInteractHandler(GameContext* context, std::function<void (GameContext&)> handler)
        {
            _context = context;
            _handler = handler;
        }
    protected:
        void handleInteractEvent(Entity& e, Entity& src)
        {
            _handler(*_context);
        }
    private:
        GameContext* _context;
        std::function<void (GameContext&)> _handler;
};
#endif
