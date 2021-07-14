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
#include "enums.h"
#include "graphics_context.h"
#include "menus/menu_manager.h"
#include "types.h"
typedef struct {
    char image[16];
    char text[64];
} DialogueStep;

typedef struct {
    unsigned int numberOfSteps;
    DialogueStep steps[8];
} Dialogue;


class GameContext
{
    public:
        Inventory* inventory;
        SceneData* sceneData = nullptr;
        GraphicsContext* graphics;
        PlayerInput input;
        MenuManager* menuManager;
        Audio audio;
        TextBox textBox;
        // Methods
        GameContext();
        ~GameContext();
		void addEntity(EntityType type);
        void addEnemy();
        void run();
        void toggleHitboxView();
        void toggleFrameRate();
        // I would like to remove this some day
        void setGameState(GameState state);
        // end
        void requestOpenTextBox(const char* image, const char* text);
        void requestOpenTextBox(TileSets t, int tile, const char* text);
        void requestOpenDialogue(const Dialogue* d);
        void openMenu(MenuType type);
        void onAllMenusClosed();
    private:
        bool _showFrameRate = false;
        bool _openTextBoxRequested = false;
        bool _openDialogueRequested = false;
        std::set<GameEvent> _gameEvents;
        std::stack<GameState> _gameState;
};
#endif
