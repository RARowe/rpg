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
            InputHandler* input,
            MovementHandler* movement,
            GraphicsHandler* graphics,
            EventHandler* event,
            std::function<void (GameContext&)> collisionHandler,
            float x,
            float y,
            int h,
            int w,
	        Direction direction,
            EntityType entityType,
            bool isCollidable,
            bool isInForeground
        );
        void processInput(KeyboardHandler& keyboard);
        void update(const float timeStep);
        void draw(const float timeStep);
        void onEvent(EventType event, Entity& src);
		int getId() const;
        float getX() const;
        float getY() const;
        int getH() const;
        int getW() const;
        const Point& getCursor();
        int getXVelocity();
        int getYVelocity();
        bool isMoving();
        bool pointInside(const Point& p) const;
		bool collidesWith(const Entity& e) const;
        void onCollision(GameContext& context);
        Direction getDirection();
        int getState() const;
        bool isEmoting() const;
        EntityType getType() const;
        bool isCollidable() const;
        bool isInForeground() const;
		void setX(float x);
		void setY(float y);
        void updateX(float x);
        void updateY(float y);
		void setXVelocity(int xVelocity);
		void setYVelocity(int yVelocity);
        void updateXVelocity(int xVelocity);
        void updateYVelocity(int yVelocity);
        void setDirection(Direction direction);
        void setVisibility(bool visible);
        void setState(int state);
        void setEmote(bool emote);
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
        std::function<void (GameContext&)> _collisionHandler;
		int _id;
        float _x;
        float _y;
        int _h;
        int _w;
        int _xVelocity;
        int _yVelocity;
        Direction _direction;
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
        Item _mostRecentlyAddedItem;
};
#endif
