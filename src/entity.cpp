#include "entity.h"
#include "game_context.h"

int Entity::ID = 0;

Entity::Entity
(
    InputHandler* input,
    MovementHandler* movement,
    GraphicsHandler* graphics,
    EventHandler* event,
    std::function<void (GameContext&)> collisionHandler,
    float x,
    float y,
    int h,
    int w,
	Direction direction,
    EntityType entityType,
    bool isCollidable,
    bool isInForeground
)
{
	_input = input;
	_movement = movement;
	_graphics = graphics;
    _event = event;
    _collisionHandler = collisionHandler;
	_id = ID++;
	_x = x;
	_y = y;
	_h = h;
	_w = w;
	_direction = direction;
	_xVelocity = 0;
	_yVelocity = 0;
    _entityType = entityType;
    _isCollidable = isCollidable;
    _isInForeground = isInForeground;
}

void Entity::processInput(KeyboardHandler& keyboard)
{
	if (_input == nullptr) { return; }
    _input->update(*this, keyboard);
}

void Entity::update(const float timeStep)
{
	if (_movement == nullptr) { return; }
    _movement->update(*this, timeStep);
}

void Entity::draw(const float timeStep)
{
    if (_graphics == nullptr || !_visible) { return; }
    _graphics->update(*this, timeStep);
}

void Entity::onEvent(EventType event, Entity& src)
{
    if (_event == nullptr) { return; }
    _event->update(*this, event, src);
}

void Entity::onCollision(GameContext& context)
{
    _collisionHandler(context);
}

static inline bool pointInEntity(const Entity& e, int x, int y)
{
	return x > e.getX() &&
        x < e.getX() + e.getW() &&
        y > e.getY() &&
        y < e.getY() + e.getH();
}

bool Entity::pointInside(const Point& p) const
{
    return pointInEntity(*this, p.x, p.y);
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

const Point& Entity::getCursor()
{
    int x = 0,
        y = 0;

    switch (_direction)
    {
        case Direction::LEFT:
            x = _x - 10;
            y = _y + (_h / 2);
            break;
        case Direction::RIGHT:
            x = _x + _w + 10;
            y = _y + (_h / 2);
            break;
        case Direction::UP:
            x = _x + (_w / 2);
            y = _y - 10;
            break;
        case Direction::DOWN:
            x = _x + (_w / 2);
            y = _y + _h + 10;
            break;
    }
    _cursor.x = x;
    _cursor.y = y;
    return _cursor;
}

int Entity::getId() const { return _id; }
float Entity::getX() const { return _x; }
float Entity::getY() const { return _y; }
int Entity::getH() const { return _h; }
int Entity::getW() const { return _w; }
int Entity::getXVelocity() { return _xVelocity; }
int Entity::getYVelocity() { return _yVelocity; }
bool Entity::isMoving() { return _xVelocity != 0 || _yVelocity != 0; }
Direction Entity::getDirection() { return _direction; }
int Entity::getState() const { return _state; }
bool Entity::isEmoting() const { return _isEmoting; }
EntityType Entity::getType() const { return _entityType; }
bool Entity::isCollidable() const { return _isCollidable; }
bool Entity::isInForeground() const { return _isInForeground; }
void Entity::setX(float x) { _x = x; }
void Entity::setY(float y) { _y = y; }
void Entity::updateX(float x) { _x += x; }
void Entity::updateY(float y) { _y += y; }
void Entity::setXVelocity(int xVelocity) { _xVelocity = xVelocity; }
void Entity::setYVelocity(int yVelocity) { _yVelocity = yVelocity; }
void Entity::updateXVelocity(int xVelocity) { _xVelocity += xVelocity; }
void Entity::updateYVelocity(int yVelocity) { _yVelocity += yVelocity; }
void Entity::setDirection(Direction direction) { _direction = direction; }
void Entity::setVisibility(bool visible) { _visible = visible; }
void Entity::setState(int state) { _state = state; }
void Entity::setEmote(bool isEmoting) { _isEmoting = isEmoting; }
// After this are player methods
void Entity::addItem(ItemType type)
{
    auto item = getItem(type);
    if (countItemInInventory(type) > 0)
    {
        _inventory[type].count++;
    }
    else
    {
        _mostRecentlyAddedItem = item;
        _inventory[type] =
        {
            1,
            item
        };
    }
    
    setState((int)PlayerStateType::ITEM_FOUND);
    resetStateAfter(3.0f);
}

void Entity::resetStateAfter(float seconds)
{
    _stateTransitionTime = seconds;
    _stateTimer = 0.0f;
}

void Entity::tick(float timeStep)
{
    if (_stateTransitionTime > 0.0f)
    {
        _stateTimer += timeStep;
        if (_stateTimer > _stateTransitionTime)
        {
            _stateTransitionTime = 0.0f;
            _stateTimer = 0.0f;
            _state = 0;
        }
    }
}

const std::map<ItemType, InventoryItem>& Entity::getInventory()
{
    return _inventory;
}

int Entity::countItemInInventory(ItemType type)
{
    return _inventory.count(type);
}

const Item& Entity::getMostRecentlyAddedItem()
{
    return _mostRecentlyAddedItem;
}

bool Entity::takeItem(ItemType type)
{
    if (countItemInInventory(type) > 0)
    {
        _inventory.erase(type);
        return true;
    }
    
    return false;
}