#ifndef PLAYER_MOVEMENT_H
#define PLAYER_MOVEMENT_H
#include <iostream>
#include "enums.h"
#include "movement_handler.h"
#include "game_context.h"
class PlayerMovement : public MovementHandler
{
    public:
        static PlayerMovement* getInstance(GameContext* context)
        {
            static PlayerMovement handler(context);
            return &handler;
        }
        void update(Entity& e, const float timeStep)
		{
		    float startX = e.pos.x;
		    float startY = e.pos.y;
			int xVelocity = e.vel.xVel;
			int yVelocity = e.vel.yVel;
		    if (xVelocity < 0)
		    {
		        e.pos.x += -120 * timeStep;
				e.vel.xVel += 2;
		    }
		    else if (xVelocity > 0)
		    {
				e.pos.x += 120 * timeStep;
				e.vel.xVel += -2;
		    }

		    if (yVelocity < 0)
		    {
				e.pos.y += -120 * timeStep;
				e.vel.yVel += 2;
		    }
		    else if (yVelocity > 0)
		    {
				e.pos.y += 120 * timeStep;
				e.vel.yVel += -2;
		    }

		    if (startX < -30)
		    {
		        e.pos.x = SCREEN_WIDTH + 30;
                _context->broadcast(EventType::CHANGE_SCENE, e);
		    } else if (startX > SCREEN_WIDTH + 30)
			{
				e.pos.x = -30;
                _context->broadcast(EventType::CHANGE_SCENE, e);
			}

		    if (startY < -30)
		    {
				e.pos.y = SCREEN_HEIGHT + 30;
                _context->broadcast(EventType::CHANGE_SCENE, e);
		    } else if (startY > SCREEN_HEIGHT + 30)
		    {
				e.pos.y = -30;
                _context->broadcast(EventType::CHANGE_SCENE, e);
		    }

            _context->resolveCollision(e, startX, startY);
		}
	private:
		PlayerMovement(GameContext* context) : _context(context) {}
		GameContext* _context;
};
#endif
