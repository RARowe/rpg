#pragma once
#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H
#include <SDL2/SDL.h>
#include <functional>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include "audio.h"
#include "menus/text_box.h"
#include "enums.h"
#include "graphics_context.h"
#include "level.h"
#include "menus/menu_manager.h"
#include "types.h"
#include "entity_factory.h"
#include "scenes.h"

class GameContext
{
    public:
        Entity* player;
        Inventory* inventory;
        Entities entities;
        SceneData* sceneData = nullptr;
        GraphicsContext* graphics;
        PlayerInput input;
        TextBox* dialog;
        MenuManager* menuManager;
        Audio audio;
        // TODO: STATE more stuff that would probably be better elsewhere
        std::map<int, float> stateTransitions;
        // Methods
        GameContext();
        ~GameContext();
        Entity* getEntityById(const int id);
        bool gameEventHasHappened(GameEvent event);
        void broadcastGameEvent(GameEvent event);
		void addEntity(EntityType type);
        void addInteraction(const InteractData& interactData);
        void addEnemy();
        bool isCollision(const Entity& e);
		void resolveCollision(Entity& e, int oldX, int oldY);
        void broadcast(EventType event, Entity& src);
        void openDialog(const char* imagePath, const char* text);
        void openTextBox(TileSets t, int tile, const char* text);
        void openTextBox(TileSets t, int tile, const std::string& text);
        void openTextBox(const std::vector<const Speech*>* speech);
        void run();
        void clearEntities();
        void toggleHitboxView();
        void toggleFrameRate();
        void loadScene(Scenes s);
        void loadScene(Scenes s, int spawnId);
        // I would like to remove this some day
        void setInputState(InputState state);
        void returnToPreviousGameState();
        // end
        void openMenu(MenuType type);
        void onAllMenusClosed();
        void closeAllMenus();
        // TODO: STATE remove this
        void registerStateTransition(Entity* e, int state, float time);
        // TODO(SCENE): These may be better in a different place
        void scene_process_interaction(GameContext* c, SceneData* s, const PlayerInput* i);
    private:
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
