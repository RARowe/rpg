#include "enums.h"
#include "movement_handler.h"

#ifndef PLAYER_MOVEMENT_H
#define PLAYER_MOVEMENT_H
class PlayerMovement : public MovementHandler
{
    public:
        void update(Entity& e)
		{
		    int startX = e.getX();
		    int startY = e.getY();
			int xVelocity = e.getXVelocity();
			int yVelocity = e.getYVelocity();
		    if (xVelocity < 0)
		    {
		        e.updateX(-2);
				e.updateXVelocity(2);
		    }
		    else if (xVelocity > 0)
		    {
				e.updateX(2);
				e.updateXVelocity(-2);
		    }

		    if (yVelocity < 0)
		    {
				e.updateY(-2);
				e.updateYVelocity(2);
		    }
		    else if (yVelocity > 0)
		    {
				e.updateY(2);
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
		}
};
#endif
