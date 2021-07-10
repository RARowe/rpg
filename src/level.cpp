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
    infile.close();
}

Level::Level(GameContext* context) : _context(context)
{
    _scene = std::make_unique<Scene>(Scene(_context));
}

void Level::load(Levels l)
{
    auto&& levelData = getLevelData(l);
    _scenes.clear();
    std::string path = "resources/game_data/levels/" + levelData.name + "/";
    std::map<int, std::string> indexMap;
    readLevelIndexFile(path + "index.txt", indexMap);

    for (Scenes s : levelData.scenes)
    {
        _scenes[s] = readSceneFile(path, indexMap[(int)s]);
    }
}

void Level::load(Scenes s)
{
    load(s, -1);
}

void Level::load(Scenes s, int spawnId)
{
    SceneData* scene = &_scenes[s];
    _scene->load(scene);

    if (spawnId > -1)
    {
        auto&& spawnBody = scene->gameEntities[scene->spawnPoints[spawnId]];
        auto player = _context->player;
        player->body.x = spawnBody.x;
        player->body.y = spawnBody.y;
    }
}

void Level::update(const float timeStep)
{
    _scene->update(timeStep);
}

void Level::draw(const TimeStep timeStep)
{
    _scene->draw(*_context->graphics, timeStep);
}
