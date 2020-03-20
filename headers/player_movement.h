#ifndef PLAYER_MOVEMENT_H
#define PLAYER_MOVEMENT_H
#include "enums.h"
#include "movement_handler.h"
#include "game_context.h"
class PlayerMovement : public MovementHandler
{
    public:
		PlayerMovement(const GameContext* context)
		{
			_context = context;
		}
        void update(Entity& e, const float timeStep)
		{
		    float startX = e.getX();
		    float startY = e.getY();
			int xVelocity = e.getXVelocity();
			int yVelocity = e.getYVelocity();
		    if (xVelocity < 0)
		    {
		        e.updateX(-120 * timeStep);
				e.updateXVelocity(2);
		    }
		    else if (xVelocity > 0)
		    {
				e.updateX(120 * timeStep);
				e.updateXVelocity(-2);
		    }

		    if (yVelocity < 0)
		    {
				e.updateY(-120 * timeStep);
				e.updateYVelocity(2);
		    }
		    else if (yVelocity > 0)
		    {
				e.updateY(120 * timeStep);
				e.updateYVelocity(-2);
		    }

		    if (startX < -30)
		    {
		        e.setX(SCREEN_WIDTH + 30);
		    } else if (startX > SCREEN_WIDTH + 30)
			{
				e.setX(-30);
			}

		    if (startY < -30)
		    {
				e.setY(SCREEN_HEIGHT + 30);
		    } else if (startY > SCREEN_HEIGHT + 30)
		    {
				e.setY(-30);
		    }

            if (_context->isCollision(e))
            {
                e.setX(startX);
                e.setY(startY);
            }
		}
	private:
		const GameContext* _context;
};
#endif
