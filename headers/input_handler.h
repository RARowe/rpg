#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include "entity.h"
#include "keyboard_handler.h"
class Entity;

class InputHandler
{
    public:
        virtual void update(Entity& e, KeyboardHandler& keyboard) = 0;
};
#endif
