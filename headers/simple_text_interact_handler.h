#ifndef SIMPLE_TEXT_INTERACT_HANDLER_H
#define SIMPLE_TEXT_INTERACT_HANDLER_H
#include "entity.h"
#include "game_context.h"
#include "interact_handler.h"

class SimpleTextInteractHandler : public InteractHandler
{
    public:
        SimpleTextInteractHandler(GameContext* context, const char* imagePath, const char* text)
        {
            _context = context;
            _imagePath = imagePath;
            _text = text;
        }
    protected:
        void handleInteractEvent(Entity& e, Entity& src)
        {
            _context->openDialog(_imagePath, _text);
        }
    private:
        GameContext* _context;
        const char* _imagePath;
        const char* _text;
};
#endif
