#ifndef TYPES_H
#define TYPES_H
#include <map>
#include <set>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#define SCREEN_HEIGHT 416
#define SCREEN_WIDTH 608

enum class EntityType {
	PLAYER,
    INTERACTION
};

enum class GameState {
    NORMAL,
    TEXTBOX,
    EDITOR,
    MODAL,
    TILE_PICKER
};

enum class Color {
    WHITE,
    BLUE,
    BLACK,
    RED
};

typedef struct {
    float x, y;
} Point;

typedef struct {
    float maxVel;
    float xVel, yVel;
} Velocity;

typedef struct {
    float x, y;
    unsigned short w, h;
} Body;

typedef struct {
    bool up, down, left, right;
    bool upClick, downClick, leftClick, rightClick;
    bool select, back, pause;
    bool r, e, b;
    bool esc;
} PlayerInput;

typedef struct {
    unsigned int tileSet;
    int tile;
    const char* imagePath = nullptr;
    std::string text;
    bool useTileset = false;
} TextBox;

typedef struct {
    unsigned int tileSet = 0;
    char name[128];
    size_t backgroundSize = 247;
    int background[247];
    size_t midgroundSize = 247;
    int midground[247];
    size_t foregroundSize = 247;
    int foreground[247];
    Velocity vel;
    std::map<int, Body> bodies;
    std::map<int, int> tileSprites;
    std::map<int, std::string> textInteractions;
    std::set<int> solidEntities;
} SceneData;

typedef struct {
    char* options[3];
    int numberOfOptions;
    int currentSelection;
    int* result;
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

inline bool point_in_body(const Body& b, const Point& p) {
	return p.x >= b.x &&
        p.x <= b.x + b.w &&
        p.y >= b.y &&
        p.y <= b.y + b.h;
}

inline bool point_in_body(const Body& b, int x, int y) {
	return x >= b.x &&
        x <= b.x + b.w &&
        y >= b.y &&
        y <= b.y + b.h;
}

inline int distance(int x1, int y1, int x2, int y2) {
    return sqrt(squared(x2 - x1) + squared(y2 -y1));
}

#define DEBUG_FRAME_RATE     1
#define DEBUG_TOGGLE_HIT_BOX 2
#define DEBUG_EDITOR 4
#endif
