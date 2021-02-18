#include "entity.h"

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
    e->body.x = x;
    e->body.y = y;
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
            body.y = position(-vel.maxVel, time, body.y);
            break;
        case Direction::DOWN:
            body.y = position(vel.maxVel, time, body.y);
            break;
        case Direction::LEFT:
            body.x= position(-vel.maxVel, time, body.x);
            break;
        case Direction::RIGHT:
            body.x= position(vel.maxVel, time, body.x);
            break;
        default:
            break;
    }
}
