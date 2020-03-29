#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H
#include <SDL2/SDL.h>
#include <vector>
#include "background_cache.h"
#include "dialog.h"
#include "enums.h"
#include "graphics_context.h"
#include "grid.h"
#include "keyboard_handler.h"
#include "types.h"
class GameContext
{
    public:
        GameContext();
        ~GameContext();
        GraphicsContext* getGraphics();
        KeyboardHandler* getKeyboardHandler();
		Entity* getEntity(EntityType type);
		bool isCollision(const Entity& e) const;
        void broadcast(EventType event, Entity& src);
        void openDialog(const char* imagePath, const char* text);
        void run();
        void hideEntities();
        void showEntities();
    private:
        GraphicsContext* _graphics;
        KeyboardHandler* _keyboard;
        Entity* _player;
        Dialog* _dialog;
        Grid* _grid;
        BackgroundCache* _cache;
		std::vector<Entity*> _entities;
};
#endif
