#pragma once
#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include "audio.h"
#include "background_cache.h"
#include "text_box.h"
#include "enums.h"
#include "graphics_context.h"
#include "grid.h"
#include "keyboard_handler.h"
#include "scene.h"
#include "types.h"
#include "script_runner.h"

class GameContext : std::enable_shared_from_this<GameContext>
{
    public:
        GameContext();
        ~GameContext();
        TextBox& getTextBox();
        GraphicsContext* getGraphics();
        KeyboardHandler* getKeyboardHandler();
        std::vector<std::shared_ptr<Entity>>& getEntities();
        std::shared_ptr<Entity> getPlayer();
		void addEntity(EntityType type);
		bool isCollision(const Entity& e) const;
        void broadcast(EventType event, Entity& src);
        void openDialog(const char* imagePath, const char* text);
        void run();
        void hideEntities();
        void showEntities();
        void runScript(ScriptType script);
        void clearEntities();
    private:
        GraphicsContext* _graphics;
        KeyboardHandler* _keyboard;
        std::shared_ptr<Entity> _player;
        TextBox* _dialog;
        Grid* _grid;
        BackgroundCache* _cache;
        ScriptRunner _scriptRunner;
		std::vector<std::shared_ptr<Entity>> _entities;
        Audio _audio;
        bool _showScene = false;
        Scene* _scene;
};
#endif
