#ifndef TRASH_INTERACT_HANDLER_H
#define TRASH_INTERACT_HANDLER_H
#include "entity.h"
#include "enums.h"
#include "interact_handler.h"
#include "game_context.h"

class TrashInteractHandler : public InteractHandler
{
    public:
        TrashInteractHandler(GameContext* context)
        {
            _context = context;
        }
    protected:
        void handleInteractEvent(Entity& e, Entity& src)
        {
            if (_numberOfInteractions != 4)
            {
                _context->openDialog("tim.png", "it trash");
            }
            else
            {
                _context->runScript(ScriptType::ITS_JUST_TRASH);
            }
            _numberOfInteractions++;
        }
    private:
        GameContext* _context;
        int _numberOfInteractions = 0;

};
#endif
