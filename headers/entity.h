#ifndef ENTITY_H
#define ENTITY_H
#include <SDL2/SDL.h>
#include "enums.h"
#include "input_handler.h"
#include "movement_handler.h"
#include "graphics_handler.h"
class Entity
{
    public:
        Entity(
                InputHandler* input,
                MovementHandler* movement,
                GraphicsHandler* graphics,
                int x,
                int y,
                int h,
                int w,
				Direction direction
              );
        void processInput(KeyboardHandler& keyboard);
        void update();
        void draw(SDL_Renderer* renderer);
        int getX();
        int getY();
        int getH();
        int getW();
        int getXVelocity();
        int getYVelocity();
        bool isMoving();
        Direction getDirection();
		void setX(int x);
		void setY(int y);
        void updateX(int x);
        void updateY(int y);
		void setXVelocity(int xVelocity);
		void setYVelocity(int yVelocity);
        void updateXVelocity(int xVelocity);
        void updateYVelocity(int yVelocity);
        void setDirection(Direction direction);
    private:
        InputHandler* _input;
        MovementHandler* _movement;
        GraphicsHandler* _graphics;
        int _x;
        int _y;
        int _h;
        int _w;
        int _xVelocity;
        int _yVelocity;
        Direction _direction;
};
#endif
