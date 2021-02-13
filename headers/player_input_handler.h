#ifndef PLAYER_INPUT_HANDLER_H
#define PLAYER_INPUT_HANDLER_H
#include "entity.h"
#include "enums.h"
#include "game_context.h"
#include "input_handler.h"
#include "keyboard_handler.h"

class PlayerInputHandler : public InputHandler
{
    public:
        static PlayerInputHandler* getInstance(GameContext* context)
        {
            static PlayerInputHandler handler(context);
            return &handler;
        }

        void update(Entity& e, KeyboardHandler& keyboard)
        {
            const int MAX_VELOCITY = 4;
            if (keyboard.isPressed(SDLK_LEFT))
            {
                e.vel.xVel = -MAX_VELOCITY;
                e.direction = Direction::LEFT;
            }
            if (keyboard.isPressed(SDLK_RIGHT))
            {
                e.vel.xVel = MAX_VELOCITY;
                e.direction = Direction::RIGHT;
            }
            if (keyboard.isPressed(SDLK_UP))
            {
                e.vel.yVel = -MAX_VELOCITY;
                e.direction = Direction::UP;
            }
            if (keyboard.isPressed(SDLK_DOWN))
            {
                e.vel.yVel = MAX_VELOCITY;
                e.direction = Direction::DOWN;
            }
            if (keyboard.isPressedAndConsume(SDLK_f))
            {
	        	_context->broadcast(EventType::INTERACT, e);
            }
        }
    private:
        PlayerInputHandler(GameContext* context) : _context(context) { }
        GameContext* _context;
};
#endif
