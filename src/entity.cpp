#include "entity.h"
#include "game_context.h"

int Entity::ID = 0;

Entity::Entity
(
    EntityType entityType,
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
    type = entityType;
	_input = input;
	_movement = movement;
	_graphics = graphics;
    _event = event;
    _collisionHandler = collisionHandler;
    pos.x = x;
    pos.y = y;
    body.w = width;
    body.h = height;
	id = ID++;
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
    if (_graphics == nullptr || !visible) { return; }
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
	if (id == e.id) { return false; }
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
            pos.y = position(-maxVelocity, time, pos.y);
            break;
        case Direction::DOWN:
            pos.y = position(maxVelocity, time, pos.y);
            break;
        case Direction::LEFT:
            pos.x= position(-maxVelocity, time, pos.x);
            break;
        case Direction::RIGHT:
            pos.x= position(maxVelocity, time, pos.x);
            break;
        default:
            break;
    }
}

const Point& Entity::getCursor()
{
    int x = 0,
        y = 0;

    switch (direction)
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
    cursor.x = x;
    cursor.y = y;
    return cursor;
}

bool Entity::isMoving() { return vel.xVel != 0 || vel.yVel != 0; }
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
    
    state = (int)PlayerStateType::ITEM_FOUND;
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
            state = 0;
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
