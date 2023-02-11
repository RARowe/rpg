#pragma once
#ifndef GAME_INTERNAL_H
#define GAME_INTERNAL_H
#include <stdbool.h>

typedef struct state_stack_t {
    int size;
    int stack[10];
} state_stack_t;

static void state_stack_init(state_stack_t *s);
static int state_stack_peek(state_stack_t *s);
static int state_stack_push(state_stack_t *s, int e);
static int state_stack_pop(state_stack_t *s);

typedef struct Point {
    float x, y;
} Point;

typedef struct Body {
    float x, y;
    int w, h;
} Body;

typedef struct {
    Body dim;
    Point textStartingPoint;
    char *options[5];
    int numberOfOptions;
    int currentSelection;
    int *result;
    bool hasTitle;
    char title[64];
} Modal;

/* Exported */
typedef struct GameData {
    // TextBox textBox;
    // Modal modal;
    // TilePicker tilePicker;
    // SceneData scene;
    // Editor editor;
    // TextEditor textEditor;
    // InteractionData interactionData;
    // float foundItem;
    // int *tile;
    state_stack_t gameState;
    // /* Title screen */
    // float flashingButton;
    // float fadeIn;
    // bool startSelected;
    // float startSelectedFlash;
    // /* End */
    bool showFrameRate;
    bool openTextBoxRequested;
    bool openModalRequested;
    bool openTilePickerRequested;
    bool openTextEditorRequested;
    bool sceneSaveRequested;
    bool sceneLoadRequested;
} GameData;

typedef enum {
    GAME_STATE_STARTUP,
    GAME_STATE_NORMAL,
    GAME_STATE_TEXTBOX,
    GAME_STATE_EDITOR,
    GAME_STATE_MODAL,
    GAME_STATE_TILE_PICKER,
    GAME_STATE_TEXT_EDITOR
} GameState;
//
// typedef struct {
// char *outBuffer;
// int cursorPos;
// char buffer[1024];
// } TextEditor;
//
//
// typedef struct {
// float maxVel;
// float xVel, yVel;
// } Velocity;
//
// typedef struct {
// float x, y;
// short w, h;
// } Body;
//
// typedef struct TextBox {
// int textureId;
// std::string text;
// } TextBox;
//
// typedef struct {
// int tile;
// char description[64];
// } Item;
//
// typedef struct SceneData {
// unsigned int tileSet = 0;
// unsigned int nextEntityId = 1;
// char name[128];
// size_t backgroundSize = 247;
// int background[247];
// size_t midgroundSize = 247;
// int midground[247];
// size_t foregroundSize = 247;
// int foreground[247];
// Point spawnPoint;
// Velocity vel;
// std::map<int, Body> bodies;
// std::map<int, int> tileSprites;
// std::map<int, std::string> textInteractions;
// std::map<int, Item> items;
// std::set<int> solidEntities;
// } SceneData;
//
// typedef struct {
// Item item;
// char text[1024];
// } InteractionData;
//
//
// typedef struct {
// unsigned int id, hTiles, vTiles, totalTiles;
// } TilesetMeta;
//
// typedef struct {
// TilesetMeta tilesetMeta;
// int tile;
// } TilePicker;
//
// static inline int squared(int x) { return x * x; }
//
// float inline position(float velocity, float time, float initialPosition) {
// return velocity * time + initialPosition;
// }
//
// inline bool point_in_body(const Body *b, const Point *p) {
// return p->x >= b->x && p->x <= b->x + b->w && p->y >= b->y &&
//    p->y <= b->y + b->h;
// }
//
// inline bool point_in_body(const Body &b, int x, int y) {
// return x >= b.x && x <= b.x + b.w && y >= b.y && y <= b.y + b.h;
// }
//
// inline bool point_in_body(const Body *b, int x, int y) {
// return x >= b->x && x <= b->x + b->w && y >= b->y && y <= b->y + b->h;
// }
//
// inline int distance(int x1, int y1, int x2, int y2) {
// return sqrt(squared(x2 - x1) + squared(y2 - y1));
// }
//
// inline int clamp_and_wrap(int i, int low, int high) {
// if (i < low) {
// return high;
// } else if (i > high) {
// return low;
// }
// return i;
// }
//
// inline bool utils_entities_collide(const Body *b1, const Body *b2) {
// int x2 = b1->x + b1->w, y2 = b1->y + b1->h, b2x2 = b2->x + b2->w,
// b2y2 = b2->y + b2->h;
// bool below = b2->y >= y2, above = b2y2 <= b1->y, left = b2x2 <= b1->x,
//  right = b2->x >= x2;
// return !(below || above || left || right);
// }
//
// /* Editor structs */
// typedef enum {
// EDITOR_MODE_NORMAL = 0,
// EDITOR_MODE_EDIT,
// EDITOR_MODE_REQUEST_RESOURCE
// } EditorMode;
//
// typedef enum {
// TOOL_SELECT = 0,
// TOOL_WALL,
// TOOL_TEXT_INTERACTION,
// TOOL_SPAWN_POINT,
// TOOL_TILE,
// TOOL_ITEM
// } Tool;
//
// typedef enum { LAYER_BACKGROUND = 0, LAYER_MIDGROUND, LAYER_FOREGROUND }
// Layer;
//
// typedef enum {
// TOOLBAR_STATE_DEFAULT = 0,
// TOOLBAR_STATE_FILE,
// TOOLBAR_STATE_TOOL,
// TOOLBAR_STATE_DEBUG
// } ToolbarState;
//
// typedef struct {
// int tile;
// int x, y;
// Layer layer;
// } TileEditor;
//
// typedef struct Editor {
// bool isInitialzed;
// EditorMode currentMode;
// state_stack_t mode;
// Body *selectedEntity;
// Tool currentTool;
// /* Cursor */
// int curX, curY;
// int startX, startY;
// int relX, relY;
// bool isDragging;
// /* Toolbar */
// ToolbarState toolBarState;
// /* Debug */
// bool showGrid;
// TileEditor tileEditor;
// /* Text edit buffer */
// int result;
// char textBuffer[1024];
// } Editor;
//
// static void game_request_open_text_box(GameData *d, unsigned int textureId,
//    const char *text);
// static void game_request_open_modal(GameData *d, char **options,
// int numberOfOptions, int *result);
// static void game_request_open_modal(GameData *d, const Body *size,
// const Point *textStartingPoint,
// const char *title, char **options,
// int numberOfOptions, int *result);
// static void game_request_open_tile_picker(GameData *d, int *tile);
// static void game_request_open_text_editor(GameData *d, char *buffer);
// static void game_request_scene_save(GameData *d);
// static void game_request_scene_load(GameData *d);
//
// /* entities.c */
// static Body *entities_get_body(SceneData *s, int entityId);
// static Body *entities_get_body_by_point(SceneData *s, float x, float y);
// static void entities_wall_add(SceneData *s, float x, float y, short w, short
// h); static void entities_wall_remove(SceneData *s, void *entity); static void
// entities_item_add(SceneData *s, void *entity, int tile,
//   const char *description);
// static int entities_item_get(SceneData *s, void *entity, Item *item);
// static void entities_text_interaction_add(SceneData *s, void *entity,
//   const char *text);
// static size_t entities_text_interaction_get(SceneData *s, void *entity,
// char *buffer);
// static void entities_spawn_point_set(SceneData *s, float x, float y);
//
// /* editor.c */
// static void editor_handle_input(GameData *d, Editor *e, Graphics *g, Input
// *i, SceneData *s);
// static void editor_draw(Editor *e, Graphics *g, float timeStep);
//
// /* modal.c */
// static int modal_handle_input(const Input *i, Modal *m);
// static void modal_draw(Graphics *g, const Modal *m, float timeStep);
// static int tile_picker_handle_input(const Input *i, TilePicker *p);
// static int text_editor_handle_input(TextEditor *t, const Input *i);
// static void tile_picker_draw(Graphics *g, const TilePicker *p);
#endif