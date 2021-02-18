#ifndef ENTITY_H
#define ENTITY_H
#include "enums.h"
#include "event_handler.h"
#include "types.h"

class GameContext;

class Entity
{
    public:
        EventHandler* event;
        int id;
        EntityType type;
        Velocity vel;
        Body body;
        Direction direction = Direction::DOWN;
        int state;
        // Refactor these simple types below
        bool isEmoting = false;
        bool isCollidable = true;
        // TODO: Maybe it would be better to have z-axis on body, and let
        //       drawer determine what the order it gets drawn in
        bool isInForeground = false;
        // end variables
        void onEvent(EventType event, Entity& src);
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
