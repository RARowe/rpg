#pragma once
#ifndef SCENES_H
#define SCENES_H
#include <functional>
#include <string>
#include <vector>
#include "enums.h"

class GameContext;

typedef struct WarpPointData
{
    int row;
    int column;
    Scenes sceneToLoad;
    int destinationWarpSpawn;
    std::string audio;
} WarpPointData;

typedef struct WarpSpawnPointData
{
    int id;
    int row;
    int column;
} WarpSpawnPointData;

typedef struct InteractData
{
    int row;
    int column;
    std::function<void (GameContext&)> interactHandler;
} InteractData;

typedef struct SceneData
{
    const std::vector<EntityType> entities;
    const std::vector<InteractData> interactions;
    const TileSets tileSet;
    const int maxNumberOfEnemies;
} SceneData;

const SceneData& getSceneData(Scenes s);
#endif