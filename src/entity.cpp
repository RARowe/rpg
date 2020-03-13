#include "entity.h"

Entity::Entity
(
    InputHandler* input,
    MovementHandler* movement,
    GraphicsHandler* graphics,
    int x,
    int y,
    int h,
    int w,
	Direction direction
)
{
	_input = input;
	_movement = movement;
	_graphics = graphics;
	_x = x;
	_y = y;
	_h = h;
	_w = w;
	_direction = direction;
	_xVelocity = 0;
	_yVelocity = 0;
}

void Entity::processInput(KeyboardHandler& keyboard)
{
	if (_input == NULL) { return; }
    _input->update(*this, keyboard);
}

void Entity::update()
{
	if (_movement == NULL) { return; }
    _movement->update(*this);
}

void Entity::draw(SDL_Renderer* renderer)
{
    _graphics->update(*this, renderer);
}

int Entity::getX() { return _x; }
int Entity::getY() { return _y; }
int Entity::getH() { return _h; }
int Entity::getW() { return _w; }
int Entity::getXVelocity() { return _xVelocity; }
int Entity::getYVelocity() { return _yVelocity; }
bool Entity::isMoving() { return _xVelocity != 0 || _yVelocity != 0; }
Direction Entity::getDirection() { return _direction; }
void Entity::setX(int x) { _x = x; }
void Entity::setY(int y) { _y = y; }
void Entity::updateX(int x) { _x += x; }
void Entity::updateY(int y) { _y += y; }
void Entity::setXVelocity(int xVelocity) { _xVelocity = xVelocity; }
void Entity::setYVelocity(int yVelocity) { _yVelocity = yVelocity; }
void Entity::updateXVelocity(int xVelocity) { _xVelocity += xVelocity; }
void Entity::updateYVelocity(int yVelocity) { _yVelocity += yVelocity; }
void Entity::setDirection(Direction direction) { _direction = direction; }
