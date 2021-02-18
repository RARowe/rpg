#ifndef ENTITY_H
#define ENTITY_H
#include <functional>
#include <map>
#include <string>
#include <vector>
#include "enums.h"
#include "event_handler.h"
#include "types.h"

class GameContext;

class Entity
{
    public:
        EventHandler* event;
        int id;
        Point pos;
        Velocity vel;
        Body body;
        Direction direction = Direction::DOWN;
        int state;
        // Refactor these simple types below
        bool visible = true;
        bool isEmoting = false;
        EntityType type;
        bool isCollidable = true;
        bool isInForeground = false;
        float maxVelocity = 0.0f;
        // end variables
        void onEvent(EventType event, Entity& src);
        bool isMoving();
        void move(Direction d, float time);
        static void initEntity(
            Entity* e, 
            EntityType type,
            EventHandler* event,
            float x,
            float y,
            int width,
            int height
        );
    private:
		static int ID;
};
#endif
