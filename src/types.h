#ifndef TYPES_H
#define TYPES_H
#include <map>
#include <set>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#define SCREEN_HEIGHT 416
#define SCREEN_WIDTH 608

enum class GameState {
    STARTUP,
    NORMAL,
    TEXTBOX,
    EDITOR,
    MODAL,
    TILE_PICKER,
    TEXT_EDITOR
};

enum class Color {
    WHITE,
    BLUE,
    BLACK,
    RED
};

typedef struct {
    char* outBuffer;
    int cursorPos;
    char buffer[1024];
} TextEditor;

typedef struct {
    float x, y;
} Point;

typedef struct {
    float maxVel;
    float xVel, yVel;
} Velocity;

typedef struct {
    float x, y;
    short w, h;
} Body;

typedef struct {
    bool up, down, left, right;
    bool upClick, downClick, leftClick, rightClick;
    bool select, back, pause;
    bool r, e, b;
    bool esc;
} PlayerInput;

typedef struct TextBox {
    int textureId;
    std::string text;
} TextBox;

typedef struct {
    int tile;
    char description[64];
} Item;

typedef struct SceneData {
    unsigned int tileSet = 0;
    unsigned int nextEntityId = 1;
    char name[128];
    size_t backgroundSize = 247;
    int background[247];
    size_t midgroundSize = 247;
    int midground[247];
    size_t foregroundSize = 247;
    int foreground[247];
    Point spawnPoint;
    Velocity vel;
    std::map<int, Body> bodies;
    std::map<int, int> tileSprites;
    std::map<int, std::string> textInteractions;
    std::map<int, Item> items;
    std::set<int> solidEntities;
} SceneData;

typedef struct {
    Item item;
    char text[1024];
} InteractionData;

typedef struct {
    Body dim;
    Point textStartingPoint;
    char* options[5];
    int numberOfOptions;
    int currentSelection;
    int* result;
    bool hasTitle;
    char title[64];
} Modal;

typedef struct {
    SDL_Texture* texture;
    unsigned int w, h;
    char name[64];
} Texture;

typedef struct {
    unsigned int id, hTiles, vTiles, totalTiles;
} TilesetMeta;

typedef struct {
    TilesetMeta tilesetMeta;
    int tile;
} TilePicker;

typedef struct RGBValues {
    Uint8 r;
    Uint8 g;
    Uint8 b;
} RGBValues;

static inline int squared(int x) {
    return x * x;
}

float inline position(float velocity, float time, float initialPosition) {
    return velocity * time + initialPosition;
}

inline bool point_in_body(const Body* b, const Point* p) {
	return p->x >= b->x &&
        p->x <= b->x + b->w &&
        p->y >= b->y &&
        p->y <= b->y + b->h;
}

inline bool point_in_body(const Body& b, int x, int y) {
	return x >= b.x &&
        x <= b.x + b.w &&
        y >= b.y &&
        y <= b.y + b.h;
}

inline bool point_in_body(const Body* b, int x, int y) {
	return x >= b->x &&
        x <= b->x + b->w &&
        y >= b->y &&
        y <= b->y + b->h;
}

inline int distance(int x1, int y1, int x2, int y2) {
    return sqrt(squared(x2 - x1) + squared(y2 -y1));
}

inline int clamp_and_wrap(int i, int low, int high) {
    if (i < low) {
        return high;
    } else if (i > high) {
        return low;
    }
    return i;
}

typedef struct {
    int size;
    int stack[10];
} state_stack_t;

void state_stack_init(state_stack_t* s);
int state_stack_peek(state_stack_t* s);
int state_stack_push(state_stack_t* s, int e);
int state_stack_pop(state_stack_t* s);

/* Editor structs */
typedef enum {
    EDITOR_MODE_NORMAL = 0,
    EDITOR_MODE_EDIT,
    EDITOR_MODE_REQUEST_RESOURCE
} EditorMode;

typedef enum {
    TOOL_SELECT = 0,
    TOOL_WALL,
    TOOL_TEXT_INTERACTION,
    TOOL_SPAWN_POINT,
    TOOL_TILE,
    TOOL_ITEM
} Tool;

typedef enum {
    LAYER_BACKGROUND = 0,
    LAYER_MIDGROUND,
    LAYER_FOREGROUND
} Layer;

typedef enum {
    TOOLBAR_STATE_DEFAULT = 0,
    TOOLBAR_STATE_FILE,
    TOOLBAR_STATE_TOOL,
    TOOLBAR_STATE_DEBUG
} ToolbarState;

typedef struct {
    int tile;
    int x, y;
    Layer layer;
} TileEditor;

typedef struct Editor {
    bool isInitialzed;
    EditorMode currentMode;
    state_stack_t mode;
    Body* selectedEntity;
    Tool currentTool;
    /* Cursor */
    int curX, curY;
    int startX, startY;
    int relX, relY;
    bool isDragging;
    /* Toolbar */
    ToolbarState toolBarState;
    /* Debug */
    bool showGrid;
    TileEditor tileEditor;
    /* Text edit buffer */
    int result;
    char textBuffer[1024];
} Editor;
#endif
