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
    pos.x = x;
    pos.y = y;
    body.w = width;
    body.h = height;
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

void Entity::draw(const TimeStep timeStep)
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
	return x >= e.pos.x &&
        x <= e.pos.x + e.body.w &&
        y >= e.pos.y &&
        y <= e.pos.y + e.body.h;
}

bool Entity::pointInside(const Point& p) const
{
    return pointInEntity(*this, p.x, p.y);
}

bool Entity::collidesWith(const Entity& e) const
{
	if (_id == e.getId()) { return false; }
    int x2 = pos.x+ body.w,
        y2 = pos.y + body.h,
        e2x2 = e.pos.x + e.body.w,
        e2y2 = e.pos.y + e.body.h;
    bool below = e.pos.y >= y2,
         above = e2y2 <= pos.y,
         left = e2x2 <= pos.x,
         right = e.pos.x >= x2;
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
            pos.y = position(-_maxVelocity, time, pos.y);
            break;
        case Direction::DOWN:
            pos.y = position(_maxVelocity, time, pos.y);
            break;
        case Direction::LEFT:
            pos.x= position(-_maxVelocity, time, pos.x);
            break;
        case Direction::RIGHT:
            pos.x= position(_maxVelocity, time, pos.x);
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
            x = pos.x - 10;
            y = pos.y + (body.h / 2);
            break;
        case Direction::RIGHT:
            x = pos.x+ body.w + 10;
            y = pos.y + (body.h / 2);
            break;
        case Direction::UP:
            x = pos.x+ (body.w / 2);
            y = pos.y - 10;
            break;
        case Direction::DOWN:
            x = pos.x+ (body.w / 2);
            y = pos.y + body.h + 10;
            break;
    }
    _cursor.x = x;
    _cursor.y = y;
    return _cursor;
}

int Entity::getId() const { return _id; }
bool Entity::isMoving() { return vel.xVel != 0 || vel.yVel != 0; }
Direction Entity::getDirection() { return _direction; }
int Entity::getState() const { return _state; }
bool Entity::isEmoting() const { return _isEmoting; }
EntityType Entity::getType() const { return _entityType; }
bool Entity::isCollidable() const { return _isCollidable; }
bool Entity::isInForeground() const { return _isInForeground; }
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
