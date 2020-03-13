#include <SDL2/SDL.h>
#include "enums.h"
#include "graphics_context.h"
#include "interactor.h"
#include "keyboard_handler.h"
#include "collision_detector.h"

#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H
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
    private:
        GraphicsContext* _graphics;
        Interactor* _interactor;
        KeyboardHandler* _keyboard;
        //CollisionDetector* _collisionDetector;
};
#endif
