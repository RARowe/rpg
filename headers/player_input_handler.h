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
        PlayerInputHandler(GameContext* context)
        {
            _context = context;
        }

        void update(Entity& e, KeyboardHandler& keyboard)
        {
            const int MAX_VELOCITY = 4;
            if (keyboard.isPressed(SDLK_LEFT))
            {
                e.setXVelocity(-MAX_VELOCITY);
                e.setDirection(Direction::LEFT);
            }
            if (keyboard.isPressed(SDLK_RIGHT))
            {
                e.setXVelocity(MAX_VELOCITY);
                e.setDirection(Direction::RIGHT);
            }
            if (keyboard.isPressed(SDLK_UP))
            {
                e.setYVelocity(-MAX_VELOCITY);
                e.setDirection(Direction::UP);
            }
            if (keyboard.isPressed(SDLK_DOWN))
            {
                e.setYVelocity(MAX_VELOCITY);
                e.setDirection(Direction::DOWN);
            }
            if (keyboard.isPressedAndConsume(SDLK_f))
            {
	        	_context->interact(0, 0);
            }
        }
    private:
        GameContext* _context;
};
#endif
