#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include "entity.h"
#include "types.h"
class Entity;

class InputHandler
{
    public:
        virtual void update(Entity& e, PlayerInput& input) = 0;
};
#endif
