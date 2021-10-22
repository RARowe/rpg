#pragma once
#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H
#include <SDL2/SDL.h>
#include <functional>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include "graphics_context.h"
#include "types.h"

class GameContext
{
    public:
        GraphicsContext* graphics;
        PlayerInput input;
        TextBox textBox;
        Modal modal;
        TilePicker tilePicker;
        // Methods
        GameContext();
        ~GameContext();
		void addEntity(EntityType type);
        void addEnemy();
        void run();
        void toggleHitboxView();
        // I would like to remove this some day
        void setGameState(GameState state);
        // end
        void requestOpenTextBox(const char* image, const char* text);
        void requestOpenTextBox(unsigned int id, int tile, const char* text);
        void requestOpenDialogue(const Dialogue* d);
        void requestOpenModal(char** options, int numberOfOptions, int* result);
        void requestOpenTilePicker(unsigned int id, int* tile);
    private:
        bool _showFrameRate = false;
        bool _openTextBoxRequested = false;
        bool _openDialogueRequested = false;
        bool _openModalRequested = false;
        bool _openTilePickerRequested = false;
        // TODO: This needs to be better
        int* _tile;
        std::stack<GameState> _gameState;
};
#endif
