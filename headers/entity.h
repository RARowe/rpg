#ifndef ENTITY_H
#define ENTITY_H
#include "enums.h"
#include "types.h"

class Entity
{
    public:
        int id;
        EntityType type;
        Velocity vel;
        Body body;
        Direction direction = Direction::DOWN;
        int state;
        // Refactor these simple types below
        bool isEmoting = false;
        bool isCollidable = true;
        // end variables
        void move(Direction d, float time);
        static void initEntity(
            Entity* e, 
            EntityType type,
            float x,
            float y,
            int width,
            int height
        );
    private:
		static int ID;
};
#endif
