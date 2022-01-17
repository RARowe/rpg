#include "types.h"

void player_process_movement(Body* body, Velocity* vel, const float timeStep) {
    float startX = body->x;
    float startY = body->y;
    int xVelocity = vel->xVel;
    int yVelocity = vel->yVel;
    if (xVelocity < 0) {
        body->x += -120 * timeStep;
    	vel->xVel += 2;
    } else if (xVelocity > 0) {
    	body->x += 120 * timeStep;
    	vel->xVel += -2;
    }
    
    if (yVelocity < 0) {
    	body->y += -120 * timeStep;
    	vel->yVel += 2;
    } else if (yVelocity > 0) {
    	body->y += 120 * timeStep;
    	vel->yVel += -2;
    }
    
    if (startX < -30) {
        body->x = SCREEN_WIDTH + 30;
    } else if (startX > SCREEN_WIDTH + 30) {
    	body->x = -30;
    }
    
    if (startY < -30) {
    	body->y = SCREEN_HEIGHT + 30;
    } else if (startY > SCREEN_HEIGHT + 30) {
    	body->y = -30;
    }
}
