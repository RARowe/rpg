#ifndef PLAYER_INPUT_HANDLER_H
#define PLAYER_INPUT_HANDLER_H
#include "entity.h"
#include "enums.h"
#include "game_context.h"
#include "input_handler.h"
#include "types.h"

class PlayerInputHandler : public InputHandler
{
    public:
        static PlayerInputHandler* getInstance(GameContext* context)
        {
            static PlayerInputHandler handler(context);
            return &handler;
        }

        void update(Entity& e, PlayerInput& i)
        {
            const int MAX_VELOCITY = 4;
            if (i.left)
            {
                e.vel.xVel = -MAX_VELOCITY;
                e.direction = Direction::LEFT;
            }
            if (i.right)
            {
                e.vel.xVel = MAX_VELOCITY;
                e.direction = Direction::RIGHT;
            }
            if (i.up)
            {
                e.vel.yVel = -MAX_VELOCITY;
                e.direction = Direction::UP;
            }
            if (i.down)
            {
                e.vel.yVel = MAX_VELOCITY;
                e.direction = Direction::DOWN;
            }
            if (i.select)
            {
	        	_context->broadcast(EventType::INTERACT, e);
            }
        }
    private:
        PlayerInputHandler(GameContext* context) : _context(context) { }
        GameContext* _context;
};
#endif
