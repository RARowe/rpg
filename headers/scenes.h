#pragma once
#ifndef SCENES_H
#define SCENES_H
#include <string>
#include <vector>
#include "enums.h"

typedef struct WarpPointData
{
    int row;
    int column;
    int destRow;
    int destColumn;
    Scenes sceneToLoad;
    std::string audio;
} WarpPointData;

typedef struct SceneData
{
    std::string path;
    std::vector<EntityType> entities;
    std::vector<WarpPointData> warpPoints;
    TileSets tileSet;
} SceneData;

const SceneData& getSceneData(Scenes s);
#endif