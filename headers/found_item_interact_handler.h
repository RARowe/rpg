#ifndef FOUND_ITEM_INTERACT_HANDLER_H
#define FOUND_ITEM_INTERACT_HANDLER_H
#include "entity.h"
#include "game_context.h"
#include "interact_handler.h"

class FoundItemInteractHandler : public InteractHandler
{
    public:
        FoundItemInteractHandler(GameContext* context)
        {
            _context = context;
        }
    protected:
        void handleInteractEvent(Entity& e, Entity& src)
        {
            if (!_itemFound)
            {
                _context->getPlayer()->addItem("encrypted compact disk");
                _context->openDialog("items.png", "Found 'Encrypted compact disk'.");
                _itemFound = !_itemFound;
            }
            else
            {
                _context->openDialog("tim.png", "It's a newspaper rack.");
            }
        }
    private:
        GameContext* _context;
        bool _itemFound = false;
};
#endif
