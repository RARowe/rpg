#ifndef PLAYER_INPUT_HANDLER_H
#define PLAYER_INPUT_HANDLER_H
#include "entity.h"
#include "enums.h"
#include "input_handler.h"
#include "keyboard_handler.h"

class PlayerInputHandler : public InputHandler
{
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
        // if (keyboard.isPressed(SDLK_f))
        // {
	    // 	int x, y;
    
	    // 	switch (_facing)
	    // 	{
	    // 		case Direction::UP:
	    // 			x = _x + 15;
	    // 			y = _y - 3;
	    // 			break;
	    // 		case Direction::DOWN:
	    // 			x = _x + 15;
	    // 			y = _y + 38;
	    // 			break;
	    // 	}
	    // 	_interactor->interact(x, y);
        // }
    }
};
#endif