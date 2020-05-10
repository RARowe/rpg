#include <cstring>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <map>
#include "game_context.h"
#include "level.h"
#include "levels.h"

using json = nlohmann::json;

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

static void readSceneFile(const std::string& path, std::vector<int>& background, std::vector<int>& midground, std::vector<int>& foreground, std::vector<WarpPointData>& warpPoints, std::vector<WarpSpawnPointData>& warpSpawnsData)
{
    background.clear();
    midground.clear();
    foreground.clear();
    warpPoints.clear();
    warpSpawnsData.clear();
    std::ifstream infile(path);
    json j;
    infile >> j;

    for (auto&& b : j["layers"])
    {
        if (b["name"] == "background")
        {
            for (int i : b["data"])
            {
                background.push_back(i - 1);
            }
        }

        if (b["name"] == "midground")
        {
            for (int i : b["data"])
            {
                midground.push_back(i - 1);
            }
        }

        if (b["name"] == "foreground")
        {
            for (int i : b["data"])
            {
                foreground.push_back(i - 1);
            }
        }

        if (b["name"] == "objects")
        {
            for (auto&& object : b["objects"])
            {
                if (object["type"] == "WARP_POINT")
                {
                    int height = object["height"];
                    int width = object["width"];
                    WarpPointData d;
                    d.row = (int)object["y"] / height;
                    d.column = (int)object["x"] / width;
                    d.destinationWarpSpawn = 0;
                    d.sceneToLoad = (Scenes)0;
                    d.audio = "audio/door.ogg";

                    for (auto&& property : object["properties"])
                    {
                        if (property["name"] == "destination_warp_spawn")
                        {
                            d.destinationWarpSpawn = (int)property["value"];
                        }
                        else if (property["name"] == "scene")
                        {
                            d.sceneToLoad = (Scenes)((int)property["value"]);
                        }
                        else
                        {
                            std::cout << "Do not recognize warp point property name: " << property["name"] << std::endl;
                        }
                    }
                    warpPoints.push_back(d);
                }
                else if (object["type"] == "WARP_SPAWN")
                {
                    int height = object["height"];
                    int width = object["width"];
                    WarpSpawnPointData warpSpawn = { 0, 0, 0 };
                    warpSpawn.row = (int)object["y"] / height;
                    warpSpawn.column = (int)object["x"] / width;
                    for (auto&& p : object["properties"])
                    {
                        if (p["name"] == "id")
                        {
                            warpSpawn.id = p["value"];
                        }
                        else
                        {
                            std::cout << "Do not recognize warp point property name: " << p["name"] << std::endl;
                        }
                    }
                    warpSpawnsData.push_back(warpSpawn);
                }
            }
        }
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
    for (Scenes s : levelData.scenes)
    {
        readSceneFile(path + indexMap[(int)s], background, midground, foreground, warpPoints, warpSpawns);
        _scenes[s] =
        {
            background,
            midground,
            foreground,
            warpPoints,
            warpSpawns
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
    _scene->load(getSceneData(s), t.background, t.objects, t.foreground, t.warpPoints, t.warpSpawns);

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