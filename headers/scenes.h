#pragma once
#ifndef SCENES_H
#define SCENES_H
#include <functional>
#include <string>
#include <vector>
#include "enums.h"
#include "types.h"

class GameContext;

typedef struct WarpSpawnPointData
{
    int row;
    int column;
    int id;
} WarpSpawnPointData;

typedef struct CollisionData
{
    int x, y, w, h;
} CollisionData;

typedef struct InteractData
{
    int row;
    int column;
    std::function<void (GameContext&)> interactHandler;
} InteractData;

typedef struct SceneData
{
    std::vector<EntityType> entities;
    std::vector<InteractData> interactions;
    TileSets tileSet;
    int maxNumberOfEnemies;
    std::vector<int> background;
    std::vector<int> midground;
    std::vector<int> foreground;
    std::vector<WarpPointData> warpPoints;
    std::vector<WarpSpawnPointData> spawnPoints;
    std::vector<CollisionData> collisions;
} SceneData;

SceneData getSceneData(Scenes s);
#endif
