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
    int destRow;
    int destColumn;
    Scenes sceneToLoad;
    std::string audio;
} WarpPointData;

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
    const std::vector<WarpPointData> warpPoints;
    const TileSets tileSet;
    const int maxNumberOfEnemies;
} SceneData;

const SceneData& getSceneData(Scenes s);
#endif