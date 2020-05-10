#ifndef SCENE_FILE_READER_H
#define SCENE_FILE_READER_H
#include <string>
#include <vector>
#include "scenes.h"

void readSceneFile
(
    const std::string& path,
    std::vector<int>& background,
    std::vector<int>& midground,
    std::vector<int>& foreground,
    std::vector<WarpPointData>& warpPoints,
    std::vector<WarpSpawnPointData>& warpSpawnsData,
    std::vector<CollisionData>& collisionData
);
#endif