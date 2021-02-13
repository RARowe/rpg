#pragma once
#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H
#include <SDL2/SDL.h>
#include <functional>
#include <set>
#include <stack>
#include <vector>
#include "audio.h"
#include "menus/text_box.h"
#include "enums.h"
#include "graphics_context.h"
#include "keyboard_handler.h"
#include "level.h"
#include "menus/menu_manager.h"
#include "types.h"
#include "script_runner.h"
#include "entity_factory.h"

class GameContext
{
    public:
        Entity* player;
        Entities entities;
        GameContext();
        ~GameContext();
        TextBox& getTextBox();
        GraphicsContext* getGraphics();
        KeyboardHandler& getKeyboardHandler();
        Audio& getAudio();
        ScriptRunner& getScriptRunner();
        MenuManager& getMenuManager();
        bool gameEventHasHappened(GameEvent event);
        void broadcastGameEvent(GameEvent event);
		void addEntity(EntityType type);
        void addWarpPoint(const WarpPointData& warpData);
        void addInteraction(const InteractData& interactData);
        void addEnemy();
        void addWarpSpawnPoint(const WarpSpawnPointData& data);
        void addCollidable(const CollisionData& data);
        bool isCollision(const Entity& e);
		void resolveCollision(Entity& e, int oldX, int oldY);
        void broadcast(EventType event, Entity& src);
        void openDialog(const char* imagePath, const char* text);
        void openTextBox(TileSets t, int tile, const std::string& text);
        void openTextBox(const std::vector<const Speech*>* speech);
        void run();
        void runScript(ScriptType script);
        void clearEntities();
        void toggleHitboxView();
        void toggleFrameRate();
        void loadScene(Scenes s);
        void loadScene(Scenes s, int spawnId);
        // I would like to remove this some day
        void setInputState(InputState state);
        void returnToPreviousGameState();
        // end
        void input();
        void openMenu(MenuType type);
        void onAllMenusClosed();
        void closeAllMenus();
    private:
        GraphicsContext* _graphics;
        KeyboardHandler* _keyboard;
        TextBox* _dialog;
        ScriptRunner _scriptRunner;
        Audio _audio;
        MenuManager* _menuManager;
        Level* _level = nullptr;
        bool _showScene = false;
        bool _showFrameRate = false;
        bool _sceneLoadRequested = false;
        int _spawnId = -1;
        Scenes _sceneToLoad;
        EntityFactory* _entityFactory = nullptr;
        std::set<GameEvent> _gameEvents;
        std::stack<InputState> _gameState;
};
#endif
