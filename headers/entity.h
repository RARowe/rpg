#ifndef ENTITY_H
#define ENTITY_H
#include <SDL2/SDL.h>
#include "enums.h"
#include "event_handler.h"
#include "input_handler.h"
#include "movement_handler.h"
#include "graphics_handler.h"
#include "types.h"
class Entity
{
    public:
        Entity
        (
            InputHandler* input,
            MovementHandler* movement,
            GraphicsHandler* graphics,
            EventHandler* event,
            int x,
            int y,
            int h,
            int w,
	        Direction direction
        );
        void processInput(KeyboardHandler& keyboard);
        void update();
        void draw(SDL_Renderer* renderer);
        void onEvent(EventType event, Entity& src);
		int getId() const;
        int getX() const;
        int getY() const;
        int getH() const;
        int getW() const;
        const Point& getCursor();
        int getXVelocity();
        int getYVelocity();
        bool isMoving();
        bool pointInside(const Point& p) const;
		bool collidesWith(const Entity& e) const;
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
        void setVisibility(bool visible);
    private:
		static int ID;
        InputHandler* _input;
        MovementHandler* _movement;
        GraphicsHandler* _graphics;
        EventHandler* _event;
		int _id;
        int _x;
        int _y;
        int _h;
        int _w;
        int _xVelocity;
        int _yVelocity;
        Direction _direction;
        Point _cursor;
        bool _visible = true;
};
#endif
