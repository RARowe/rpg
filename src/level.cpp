#include <fstream>
#include <nlohmann/json.hpp>
#include "game_context.h"
#include "level.h"
#include "levels.h"

using json = nlohmann::json;

static void readJSONFile(const std::string& path, std::vector<int>& background, std::vector<int>& objects, std::vector<int>& foreground)
{
    background.clear();
    objects.clear();
    foreground.clear();
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

        if (b["name"] == "objects")
        {
            for (int i : b["data"])
            {
                objects.push_back(i - 1);
            }
        }

        if (b["name"] == "foreground")
        {
            for (int i : b["data"])
            {
                foreground.push_back(i - 1);
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

    std::vector<int> background;
    std::vector<int> objects;
    std::vector<int> foreground;
    for (Scenes s : levelData.scenes)
    {
        readJSONFile(path + std::to_string((int)s) + ".json", background, objects, foreground);
        _scenes[s] =
        {
            background,
            objects,
            foreground
        };
    }
}


void Level::load(Scenes s)
{
    const TileData& t = _scenes[s];
    _scene->load(getSceneData(s), t.background, t.objects, t.foreground);
}

void Level::update(const float timeStep)
{
    _scene->update(timeStep);
}

void Level::draw(const float timeStep)
{
    _scene->draw(*_context->getGraphics(), timeStep);
}