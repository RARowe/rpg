#include "entity.h"

int Entity::ID = 0;

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
	_id = ID++;
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

static inline bool pointInEntity(const Entity& e, int x, int y)
{
	return x > e.getX() &&
        x < e.getX() + e.getW() &&
        y > e.getY() &&
        y < e.getY() + e.getH();
}

bool Entity::collidesWith(const Entity& e) const
{
	if (_id == e.getId()) { return false; }
	int x2 = _x + _w, y2 = _y + _h;
    int e2x2 = e.getX() + e.getW(), e2y2 = e.getY() + e.getH();
	
	return pointInEntity(e, _x, _y) ||
		pointInEntity(e, x2, _y) ||
		pointInEntity(e, _x, y2) ||
		pointInEntity(e, x2, y2) ||
	    pointInEntity(*this, e.getX(), e.getY()) ||
		pointInEntity(*this, e2x2, e.getY()) ||
		pointInEntity(*this, e.getX(), e2y2) ||
		pointInEntity(*this, e2x2, e2y2);
}

int Entity::getId() const { return _id; }
int Entity::getX() const { return _x; }
int Entity::getY() const { return _y; }
int Entity::getH() const { return _h; }
int Entity::getW() const { return _w; }
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
