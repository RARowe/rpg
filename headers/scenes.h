#pragma once
#ifndef SCENES_H
#define SCENES_H
#include <string>
#include <vector>
#include "enums.h"

typedef struct SceneData
{
    std::string path;
    std::vector<EntityType> entities;
} SceneData;

const SceneData& getSceneData(Scenes s);
#endif