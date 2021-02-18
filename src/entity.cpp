#include "entity.h"
#include "game_context.h"

int Entity::ID = 0;

void Entity::initEntity(
    Entity* e,
    EntityType entityType,
    EventHandler* event,
    float x,
    float y,
    int width,
    int height
){
    e->type = entityType;
    e->event = event;
    e->pos.x = x;
    e->pos.y = y;
    e->body.w = width;
    e->body.h = height;
    e->isCollidable = true;
	e->id = ID++;
}

void Entity::onEvent(EventType eventType, Entity& src)
{
    if (event == nullptr) { return; }
    event->update(*this, eventType, src);
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
            pos.y = position(-vel.maxVel, time, pos.y);
            break;
        case Direction::DOWN:
            pos.y = position(vel.maxVel, time, pos.y);
            break;
        case Direction::LEFT:
            pos.x= position(-vel.maxVel, time, pos.x);
            break;
        case Direction::RIGHT:
            pos.x= position(vel.maxVel, time, pos.x);
            break;
        default:
            break;
    }
}
