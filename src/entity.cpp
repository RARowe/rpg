#include "entity.h"
#include "game_context.h"

int Entity::ID = 0;

void Entity::initEntity(
    Entity* e,
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
){
    e->type = entityType;
	e->input = input;
	e->movement = movement;
	e->graphics = graphics;
    e->event = event;
    e->collisionHandler = collisionHandler;
    e->pos.x = x;
    e->pos.y = y;
    e->body.w = width;
    e->body.h = height;
    e->visible = true;
    e->isCollidable = true;
	e->id = ID++;
    // TODO: remove this
    if (entityType == EntityType::PLAYER) {
        e->inventory = new std::map<ItemType, InventoryItem>();
    }
}

void Entity::processInput(KeyboardHandler& keyboard)
{
	if (input == nullptr) { return; }
    input->update(*this, keyboard);
}

void Entity::update(const float timeStep)
{
	if (movement == nullptr) { return; }
    movement->update(*this, timeStep);
}

void Entity::draw(const TimeStep timeStep)
{
    if (graphics == nullptr || !visible) { return; }
    graphics->update(*this, timeStep);
}

void Entity::onEvent(EventType eventType, Entity& src)
{
    if (event == nullptr) { return; }
    event->update(*this, eventType, src);
}

void Entity::onCollision(GameContext& context, Entity& e)
{
    collisionHandler(context, *this, e);
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


bool Entity::isMoving() { return vel.xVel != 0 || vel.yVel != 0; }
// After this are player methods
void Entity::addItem(ItemType type)
{
    auto item = getItem(type);
    if (countItemInInventory(type) > 0)
    {
        (*inventory)[type].count++;
    }
    else
    {
        _mostRecentlyAddedItem = item;
        (*inventory)[type] =
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
    return *inventory;
}

int Entity::countItemInInventory(ItemType type)
{
    return inventory->count(type);
}

const Item& Entity::getMostRecentlyAddedItem()
{
    return _mostRecentlyAddedItem;
}

bool Entity::takeItem(ItemType type)
{
    if (countItemInInventory(type) > 0)
    {
        inventory->erase(type);
        return true;
    }
    
    return false;
}
