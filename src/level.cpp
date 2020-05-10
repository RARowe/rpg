#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include "game_context.h"
#include "level.h"
#include "levels.h"
#include "scene_file_reader.h"

static void readLevelIndexFile(const std::string& path, std::map<int, std::string>& indexMap)
{
    std::ifstream infile(path);
    std::string line;
    int key;
    char buffer[128];

    while (std::getline(infile, line))
    {
        std::sscanf(line.c_str(), "%d,%s", &key, buffer);
        indexMap[key] = std::string(buffer);
    }
}

Level::Level(GameContext* context) : _context(context)
{
    _scene = std::make_unique<Scene>(Scene(_context));
}

void Level::load(Levels l)
{
    auto&& levelData = getLevelData(l);
    _scenes.clear();
    std::string path = "resources/levels/" + levelData.name + "/";
    std::map<int, std::string> indexMap;
    readLevelIndexFile(path + "index.txt", indexMap);

    std::vector<int> background;
    std::vector<int> midground;
    std::vector<int> foreground;
    std::vector<WarpPointData> warpPoints;
    std::vector<WarpSpawnPointData> warpSpawns;
    std::vector<CollisionData> collisions;
    for (Scenes s : levelData.scenes)
    {
        readSceneFile(path + indexMap[(int)s], background, midground, foreground, warpPoints, warpSpawns, collisions);
        _scenes[s] =
        {
            background,
            midground,
            foreground,
            warpPoints,
            warpSpawns,
            collisions
        };
    }
}

void Level::load(Scenes s)
{
    load(s, -1);
}

void Level::load(Scenes s, int spawnId)
{
    const TileData& t = _scenes[s];
    _scene->load(getSceneData(s), t.background, t.objects, t.foreground, t.warpPoints, t.warpSpawns, t.collisionData);

    if (spawnId > -1)
    {
        for (auto&& s : t.warpSpawns)
        {
            if (s.id == spawnId)
            {
                auto player = _context->getPlayer();
                player->setX(s.column * 32);
                player->setY(s.row * 32);
            }
        }
    }
}

void Level::update(const float timeStep)
{
    _scene->update(timeStep);
}

void Level::draw(const float timeStep)
{
    _scene->draw(*_context->getGraphics(), timeStep);
}