#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H
#include <SDL2/SDL.h>
#include <vector>
#include "background_cache.h"
#include "text_box.h"
#include "enums.h"
#include "graphics_context.h"
#include "grid.h"
#include "keyboard_handler.h"
#include "types.h"
#include "script_runner.h"

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
        void runScript(ScriptType script);
    private:
        GraphicsContext* _graphics;
        KeyboardHandler* _keyboard;
        Entity* _player;
        TextBox* _dialog;
        Grid* _grid;
        BackgroundCache* _cache;
        ScriptRunner _scriptRunner;
		std::vector<Entity*> _entities;
};
#endif
