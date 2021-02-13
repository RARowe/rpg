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
#include "graphics_handler.h"
#include "types.h"

class GameContext;

class Entity
{
    public:
        Entity
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
        );
        Point pos;
        Velocity vel;
        Body body;
        void processInput(KeyboardHandler& keyboard);
        void update(const float timeStep);
        void draw(const TimeStep timeStep);
        void onEvent(EventType event, Entity& src);
		int getId() const;
        const Point& getCursor();
        bool isMoving();
        bool pointInside(const Point& p) const;
		bool collidesWith(const Entity& e) const;
        void onCollision(GameContext& context, Entity& e);
        Direction getDirection();
        int getState() const;
        bool isEmoting() const;
        EntityType getType() const;
        bool isCollidable() const;
        bool isInForeground() const;
        void move(Direction d, float time);
        void setCollidable(bool collidable);
        void setIsInForeground(bool isInForeground);
        void setDirection(Direction direction);
        void setVisibility(bool visible);
        void setState(int state);
        void setEmote(bool emote);
        void setMaxVelocity(float velocity);
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
        InputHandler* _input;
        MovementHandler* _movement;
        GraphicsHandler* _graphics;
        EventHandler* _event;
        std::function<void (GameContext&, Entity&, Entity&)> _collisionHandler;
		int _id;
        Direction _direction = Direction::DOWN;
        Point _cursor;
        bool _visible = true;
        int _state = 0;
        bool _isEmoting = false;
        EntityType _entityType;
        bool _isCollidable = true;
        bool _isInForeground = false;
        // after this are player specific fields
        std::map<ItemType, InventoryItem> _inventory;
        float _stateTransitionTime = 0.0f;
        float _stateTimer = 0.0f;
        float _maxVelocity = 0.0f;
        Item _mostRecentlyAddedItem;
};
#endif
