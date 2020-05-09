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

static void readSceneFile(const std::string& path, std::vector<int>& background, std::vector<int>& midground, std::vector<int>& foreground, std::vector<WarpPointData>& warpPoints)
{
    background.clear();
    midground.clear();
    foreground.clear();
    warpPoints.clear();
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
                    d.destRow = 0;
                    d.destColumn = 0;
                    d.sceneToLoad = (Scenes)0;
                    d.audio = "audio/door.ogg";

                    for (auto&& property : object["properties"])
                    {
                        if (property["name"] == "column_destination")
                        {
                            d.destColumn = (int)property["value"];
                        }
                        else if (property["name"] == "row_destination")
                        {
                            d.destRow = (int)property["value"];
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
    for (Scenes s : levelData.scenes)
    {
        readSceneFile(path + indexMap[(int)s], background, midground, foreground, warpPoints);
        _scenes[s] =
        {
            background,
            midground,
            foreground,
            warpPoints
        };
    }
}


void Level::load(Scenes s)
{
    const TileData& t = _scenes[s];
    _scene->load(getSceneData(s), t.background, t.objects, t.foreground, t.warpPoints);
}

void Level::update(const float timeStep)
{
    _scene->update(timeStep);
}

void Level::draw(const float timeStep)
{
    _scene->draw(*_context->getGraphics(), timeStep);
}