#pragma once
#ifndef SCENES_H
#define SCENES_H
#include <functional>
#include <string>
#include <map>
#include <set>
#include <vector>
#include "enums.h"
#include "types.h"

class GameContext;

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
    std::vector<int> background;
    std::vector<int> midground;
    std::vector<int> foreground;
    std::vector<Body> gameEntities;
    std::map<int, int> tileSprites;
    std::map<int, std::string> textInteractions;
    int interactionCounter;
    std::set<int> solidEntities;
    std::map<int, EnemySpawnZone> enemySpawnZones;
    std::map<int, WarpPoint> warpPoints;
    std::map<int,int> spawnPoints; // spawnId -> entityId
} SceneData;

SceneData getSceneData(Scenes s);
#endif
