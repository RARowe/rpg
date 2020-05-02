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
    std::string path;
    std::vector<EntityType> entities;
    std::vector<InteractData> interactions;
    std::vector<WarpPointData> warpPoints;
    TileSets tileSet;
} SceneData;

const SceneData& getSceneData(Scenes s);
#endif