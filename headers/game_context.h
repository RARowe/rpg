#pragma once
#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H
#include <SDL2/SDL.h>
#include <stack>
#include "graphics.h"
#include "types.h"

class GameContext {
    public:
        Graphics graphics;
        PlayerInput input;
        TextBox textBox;
        Modal modal;
        TilePicker tilePicker;
        SceneData scene;
        void run();
        void requestOpenTextBox(const char* image, const char* text);
        void requestOpenModal(char** options, int numberOfOptions, int* result);
        void requestOpenTilePicker(unsigned int id, int* tile);
        void requestSceneSave();
        void requestSceneLoad();
    private:
        bool _showFrameRate = false;
        bool _openTextBoxRequested = false;
        bool _openModalRequested = false;
        bool _openTilePickerRequested = false;
        bool _sceneSaveRequested = false;
        bool _sceneLoadRequested = false;
        // TODO: This needs to be better
        int* _tile;
        std::stack<GameState> _gameState;
};
#endif
