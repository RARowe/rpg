#ifndef TYPES_H
#define TYPES_H
#include <map>
#include <set>
#include <string>
#include <vector>
#include "enums.h"

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
} PlayerInput;

typedef struct {
    uint8_t size;
    ItemType mostRecentlyAdded;
    ItemType items[];
} Inventory;

typedef struct {
    Scenes sceneToLoad;
    int destinationSpawn;
} WarpPoint;

typedef struct {
    unsigned int tileSet;
    int tile;
    const char* imagePath = nullptr;
    std::string text;
    bool useTileset = false;
} TextBox;

typedef struct {
    unsigned int tileSet = 0;
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
    std::map<int, WarpPoint> warpPoints;
    std::map<int,int> spawnPoints; // spawnId -> entityId
} SceneData;

#define DEBUG_FRAME_RATE     1
#define DEBUG_TOGGLE_HIT_BOX 2
#define DEBUG_EDITOR 4
#endif
