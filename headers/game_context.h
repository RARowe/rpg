#pragma once
#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H
#include <SDL2/SDL.h>
#include <functional>
#include <memory>
#include <vector>
#include "audio.h"
#include "background_cache.h"
#include "text_box.h"
#include "enums.h"
#include "graphics_context.h"
#include "grid.h"
#include "keyboard_handler.h"
#include "pause_menu.h"
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
        KeyboardHandler& getKeyboardHandler();
        std::vector<std::shared_ptr<Entity>>& getEntities();
        std::shared_ptr<Entity> getPlayer();
        Audio& getAudio();
        ScriptRunner& getScriptRunner();
		void addEntity(EntityType type);
		void resolveCollision(Entity& e, int oldX, int oldY);
        void broadcast(EventType event, Entity& src);
        void openDialog(const char* imagePath, const char* text);
        void run();
        void hideEntities();
        void showEntities();
        void runScript(ScriptType script);
        void clearEntities();
        void loadObjectLayerCollisionDetection(const std::vector<int>& objectLayer);
        void toggleHitboxView();
        void toggleFrameRate();
        // I would like to remove this some day
        void setInputState(InputState state);
        // end
        void pause();
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
        PauseMenu* _pauseMenu;
        std::function<void (GameContext&)> _inputState;
        bool _showScene = false;
        bool _showFrameRate = false;
        Scene* _scene;
};
#endif
