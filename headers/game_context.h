#pragma once
#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H
#include <SDL2/SDL.h>
#include <functional>
#include <memory>
#include <set>
#include <stack>
#include <vector>
#include "audio.h"
#include "text_box.h"
#include "enums.h"
#include "graphics_context.h"
#include "keyboard_handler.h"
#include "menus/menu_manager.h"
#include "scene.h"
#include "scenes.h"
#include "types.h"
#include "script_runner.h"
#include "entity_factory.h"

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
        MenuManager& getMenuManager();
        bool gameEventHasHappened(GameEvent event);
        void broadcastGameEvent(GameEvent event);
		void addEntity(EntityType type);
        void addWarpPoint(const WarpPointData& warpData);
        void addInteraction(const InteractData& interactData);
		void resolveCollision(Entity& e, int oldX, int oldY);
        void broadcast(EventType event, Entity& src);
        void openDialog(const char* imagePath, const char* text);
        void openTextBox(TileSets t, int tile, const std::string& text);
        void run();
        void runScript(ScriptType script);
        void clearEntities();
        void loadObjectLayerCollisionDetection(const std::vector<int>& objectLayer);
        void toggleHitboxView();
        void toggleFrameRate();
        void loadScene(const SceneData& data);
        // I would like to remove this some day
        void setInputState(InputState state);
        // end
        void input();
        void openMenu(MenuType type);
        void onAllMenusClosed();
        void closeAllMenus();
    private:
        GraphicsContext* _graphics;
        KeyboardHandler* _keyboard;
        std::shared_ptr<Entity> _player;
        TextBox* _dialog;
        ScriptRunner _scriptRunner;
		std::vector<std::shared_ptr<Entity>> _entities;
        Audio _audio;
        MenuManager* _menuManager;
        bool _showScene = false;
        bool _showFrameRate = false;
        Scene* _scene = nullptr;
        EntityFactory* _entityFactory = nullptr;
        std::set<GameEvent> _gameEvents;
        std::stack<InputState> _gameState;
};
#endif
