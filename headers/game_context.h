#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H
#include <SDL2/SDL.h>
#include <vector>
#include "dialog.h"
#include "enums.h"
#include "graphics_context.h"
#include "interactor.h"
#include "keyboard_handler.h"
#include "types.h"
class GameContext
{
    public:
        GameContext();
        ~GameContext();
        GraphicsContext* getGraphics();
        Interactor* getInteractor();
        KeyboardHandler* getKeyboardHandler();
		Entity* getEntity(EntityType type);
		bool isCollision(const Entity& e) const;
        void broadcast(EventType event, Entity& src);
        void openDialog(const char* imagePath, const char* text);
        void run();
    private:
        GraphicsContext* _graphics;
        Interactor* _interactor;
        KeyboardHandler* _keyboard;
        Entity* _player;
        Dialog* _dialog;
		std::vector<Entity*> _entities;
};
#endif
