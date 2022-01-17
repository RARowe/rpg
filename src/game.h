#pragma once
#ifndef GAME_H
#define GAME_H
#include "graphics.h"
#include "types.h"

/* Exported */
typedef struct GameData {
    bool isInitialized;
    TextBox textBox;
    Modal modal;
    TilePicker tilePicker;
    SceneData scene;
    Editor editor;
    TextEditor textEditor;
    InteractionData interactionData;
    float foundItem;
    int* tile;
    state_stack_t gameState;
    /* Title screen */
    float flashingButton = 0.0f;
    float fadeIn = 0.0f;
    bool startSelected = false;
    float startSelectedFlash = 0.0f;
    /* End */
    bool showFrameRate = false;
    bool openTextBoxRequested = false;
    bool openModalRequested = false;
    bool openTilePickerRequested = false;
    bool openTextEditorRequested = false;
    bool sceneSaveRequested = false;
    bool sceneLoadRequested = false;
} GameData;

void game_run_frame(GameData* d, Graphics* g, Audio* a, Input* i, float timeStep);

/* Internal */
static void game_init(GameData* d);
static void game_request_open_text_box(GameData* d, unsigned int textureId, const char* text);
static void game_request_open_modal(GameData* d, char** options, int numberOfOptions, int* result);
static void game_request_open_modal(GameData* d, const Body* size, const Point* textStartingPoint, const char* title, char** options, int numberOfOptions, int* result);
static void game_request_open_tile_picker(GameData* d, int* tile);
static void game_request_open_text_editor(GameData* d, char* buffer);
static void game_request_scene_save(GameData* d);
static void game_request_scene_load(GameData* d);
#endif
