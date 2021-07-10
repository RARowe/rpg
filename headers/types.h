#ifndef TYPES_H
#define TYPES_H
#include <string>
#include <vector>
#include "enums.h"

class Entity;

typedef struct Point
{
    float x, y;
} Point;

typedef struct Velocity
{
    float maxVel;
    float xVel, yVel;
} Velocity;

typedef struct Body {
    float x, y;
    unsigned short w, h;
} Body;

typedef struct Entities {
    unsigned short size;
    unsigned short back;
    Entity* entities;
} Entities;

// TODO: Should this be refactored to bitmasks?
//       I don't know what is more efficient.
typedef struct {
    bool up, down, left, right;
    bool upClick, downClick, leftClick, rightClick;
    bool select, back, pause;
    uint8_t debug;
} PlayerInput;

typedef struct {
    uint8_t size;
    ItemType mostRecentlyAdded;
    ItemType items[];
} Inventory;

typedef struct
{
    Scenes sceneToLoad;
    int destinationSpawn;
} WarpPoint;

typedef struct {
    int currentNumberOfEnemies;
    int maxNumberOfEnemies;
} EnemySpawnZone;

typedef struct {
    TileSets tileSet;
    int tile;
    const char* imagePath = nullptr;
    std::string text;
    bool useTileset = false;
} TextBox;
typedef struct SceneData
{
    //std::vector<EntityType> entities;
    //std::vector<InteractData> interactions;
    TileSets tileSet;
    std::vector<int> background;
    std::vector<int> midground;
    std::vector<int> foreground;
    std::vector<Body> gameEntities;
    std::map<int, int> tileSprites;
    std::map<int, std::string> textInteractions;
    std::set<int> solidEntities;
    std::map<int, EnemySpawnZone> enemySpawnZones;
    std::map<int, WarpPoint> warpPoints;
    std::map<int,int> spawnPoints; // spawnId -> entityId
} SceneData;

#define DEBUG_FRAME_RATE     1
#define DEBUG_TOGGLE_HIT_BOX 2
#define DEBUG_EDITOR 4
#endif
