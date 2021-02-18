#ifndef INTERACT_HANDLER_H
#define INTERACT_HANDLER_H
#include "entity.h"
#include "enums.h"
#include "event_handler.h"
#include "game_context.h"
#include "types.h"
#include <iostream>

// TODO: This could be in better place
static inline bool pointInEntity(const Entity& e, const Point& p)
{
	return p.x >= e.body.x &&
        p.x <= e.body.x + e.body.w &&
        p.y >= e.body.y &&
        p.y <= e.body.y + e.body.h;
}

static inline void calculateCursor(Point& c, const Entity& e)
{
    switch (e.direction)
    {
        case Direction::LEFT:
            c.x = e.body.x - 10;
            c.y = e.body.y + (e.body.h / 2);
            break;
        case Direction::RIGHT:
            c.x = e.body.x + e.body.w + 10;
            c.y = e.body.y + (e.body.h / 2);
            break;
        case Direction::UP:
            c.x = e.body.x + (e.body.w / 2);
            c.y = e.body.y - 10;
            break;
        case Direction::DOWN:
            c.x = e.body.x + (e.body.w / 2);
            c.y = e.body.y + e.body.h + 10;
            break;
    }
}

class InteractHandler : public EventHandler
{
    public:
        Point cursor;
        void update(Entity& e, EventType event, Entity& src)
        {
            calculateCursor(cursor, src);
            if (event == EventType::INTERACT && pointInEntity(e, cursor))
            {
                handleInteractEvent(e, src);
            }
        }
    protected:
        virtual void handleInteractEvent(Entity& e, Entity& src) = 0;
};
#endif
