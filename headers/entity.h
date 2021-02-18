#ifndef ENTITY_H
#define ENTITY_H
#include <functional>
#include <map>
#include <string>
#include <vector>
#include "enums.h"
#include "event_handler.h"
#include "input_handler.h"
#include "items.h"
#include "movement_handler.h"
#include "types.h"

class GameContext;


class Entity
{
    public:
        InputHandler* input;
        MovementHandler* movement;
        EventHandler* event;
        std::function<void (GameContext&, Entity&, Entity&)> collisionHandler;
        int id;
        Point pos;
        Velocity vel;
        Body body;
        Direction direction = Direction::DOWN;
        std::map<ItemType, InventoryItem>* inventory;
        // Refactor these simple types below
        bool visible = true;
        int state = 0;
        bool isEmoting = false;
        EntityType type;
        bool isCollidable = true;
        bool isInForeground = false;
        float maxVelocity = 0.0f;
        // end variables
        void processInput(PlayerInput& i);
        void update(const float timeStep);
        void onEvent(EventType event, Entity& src);
        bool isMoving();
        void onCollision(GameContext& context, Entity& e);
        void move(Direction d, float time);
        static void initEntity(
            Entity* e, 
            EntityType type,
            InputHandler* input,
            MovementHandler* movement,
            EventHandler* event,
            std::function<void (GameContext&, Entity&, Entity&)> collisionHandler,
            float x,
            float y,
            int width,
            int height
        );
        // After this are player specific methods
        void addItem(ItemType item);
        void resetStateAfter(float seconds);
        void tick(float timeStep);
        const std::map<ItemType, InventoryItem>& getInventory();
        int countItemInInventory(ItemType type);
        bool takeItem(ItemType type);
        const Item& getMostRecentlyAddedItem();
    private:
		static int ID;
        // after this are player specific fields
        float _stateTransitionTime = 0.0f;
        float _stateTimer = 0.0f;
        Item _mostRecentlyAddedItem;
};
#endif
