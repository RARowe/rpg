#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H
#include <SDL2/SDL.h>
#include <vector>
#include "enums.h"
#include "graphics_context.h"
#include "interactor.h"
#include "keyboard_handler.h"
#include "collision_detector.h"
class GameContext
{
    public:
        GameContext();
        ~GameContext();
        GraphicsContext* getGraphics();
        Interactor* getInteractor();
        KeyboardHandler* getKeyboardHandler();
        //CollisionDetector* getCollisionDetector();
		Entity* getEntity(EntityType type);
		bool isCollision(const Entity& e) const;
    private:
        GraphicsContext* _graphics;
        Interactor* _interactor;
        KeyboardHandler* _keyboard;
		std::vector<Entity*> _entities;
        //CollisionDetector* _collisionDetector;
};
#endif
