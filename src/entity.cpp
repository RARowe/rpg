#include "entity.h"
#include "game_context.h"

int Entity::ID = 0;

Entity::Entity
(
    EntityType type,
    InputHandler* input,
    MovementHandler* movement,
    GraphicsHandler* graphics,
    EventHandler* event,
    std::function<void (GameContext&, Entity&, Entity&)> collisionHandler,
    float x,
    float y,
    int width,
    int height
)
{
    _entityType = type;
	_input = input;
	_movement = movement;
	_graphics = graphics;
    _event = event;
    _collisionHandler = collisionHandler;
    _x = x;
    _y = y;
    _w = width;
    _h = height;
	_id = ID++;
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

void Entity::onCollision(GameContext& context, Entity& e)
{
    _collisionHandler(context, *this, e);
}

static inline bool pointInEntity(const Entity& e, int x, int y)
{
	return x >= e.getX() &&
        x <= e.getX() + e.getW() &&
        y >= e.getY() &&
        y <= e.getY() + e.getH();
}

bool Entity::pointInside(const Point& p) const
{
    return pointInEntity(*this, p.x, p.y);
}

bool Entity::collidesWith(const Entity& e) const
{
	if (_id == e.getId()) { return false; }
    int x2 = _x + _w,
        y2 = _y + _h,
        e2x2 = e._x + e._w,
        e2y2 = e._y + e._h;
    bool below = e._y >= y2,
         above = e2y2 <= _y,
         left = e2x2 <= _x,
         right = e._x >= x2;
    return !(below || above || left || right);
}

float inline position(float velocity, float time, float initialPosition)
{
    return velocity * time + initialPosition;
}

void Entity::move(Direction d, float time)
{
    switch (d)
    {
        case Direction::UP:
            _y = position(-_maxVelocity, time, _y);
            break;
        case Direction::DOWN:
            _y = position(_maxVelocity, time, _y);
            break;
        case Direction::LEFT:
            _x = position(-_maxVelocity, time, _x);
            break;
        case Direction::RIGHT:
            _x = position(_maxVelocity, time, _x);
            break;
        default:
            break;
    }
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
void Entity::setCollidable(bool collidable) { _isCollidable = collidable; }
void Entity::setIsInForeground(bool isInForeground) { _isInForeground = isInForeground; }
void Entity::setMaxVelocity(float velocity) { _maxVelocity = velocity; }
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