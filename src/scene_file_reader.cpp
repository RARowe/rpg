#include "scene_file_reader.h"
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include "pugixml.hpp"
#define SCALING_FACTOR 2

class TemplateCache
{
    public:
        const pugi::xml_node& get(const std::string& path)
        {
            if (_templates.count(path) == 0)
            {
                pugi::xml_document doc;
                _templates[path] = std::make_unique<pugi::xml_document>();
                auto result = _templates[path]->load_file(path.c_str());

                if (result.status != pugi::status_ok)
                {
                    std::cout << result.description() << ": " << path << std::endl;
                }
            }

            return _templates[path]->child("template").child("object");
        }
    private:
        std::map<std::string, std::unique_ptr<pugi::xml_document>> _templates;
};

typedef struct ReaderContext
{
    SceneData scene;
    std::map<std::string, pugi::xml_document> templates;
} ReaderContext;

static void readLayerCSVData(const std::string& csvData, std::vector<int>& container);
static void readLayerData(const pugi::xml_node& root, ReaderContext& context);
static void readSceneFile(const std::string& path, ReaderContext& context);
static void readObjectData(const pugi::xml_node& root, ReaderContext& context);
static void readWarpPoint(const pugi::xml_node& warpPointData, ReaderContext& context);
static void readWarpSpawnData(const pugi::xml_node& spawnPointData, ReaderContext& context);
static void readCollisionData(const pugi::xml_node& collisionData, ReaderContext& context);

static void readLayerCSVData(const std::string& csvData, std::vector<int>& container)
{
    std::stringstream stream(csvData);
    std::string value;
    while (std::getline(stream, value, ','))
    {
        container.push_back(std::stoi(value) - 1);
    }
}

static void readLayerData(const pugi::xml_node& root, ReaderContext& context)
{
    auto& data = context.scene;
    for (pugi::xml_node layer : root.children("layer"))
    {
        std::string csv_data(layer.child_value("data"));
        std::string name(layer.attribute("name").value());

        if (name == "background") { readLayerCSVData(csv_data, data.background); }
        else if (name == "midground") { readLayerCSVData(csv_data, data.midground); }
        else if (name == "foreground") { readLayerCSVData(csv_data, data.foreground); }
        else { std::cout << "Unrecognized layer name: " << name << std::endl; }
    }
}


static void readWarpPoint(const pugi::xml_node& warpPointData, ReaderContext& context)
{
    auto& data = context.scene;
    WarpPointData warpData =
    {
        (warpPointData.attribute("y").as_int() * SCALING_FACTOR) / 32,
        (warpPointData.attribute("x").as_int() * SCALING_FACTOR) / 32
    };
    warpData.audio = "audio/door.ogg";

    for (auto&& p : warpPointData.child("properties").children())
    {
        auto name = std::string(p.attribute("name").value());

        if (name == "destination_warp_spawn") { warpData.destinationWarpSpawn = p.attribute("value").as_int(); }
        else if (name == "scene") { warpData.sceneToLoad = (Scenes)p.attribute("value").as_int(); }
        else { std::cout << "Attribute name: '" << name << "' not recognized for warp point." << std::endl; }
    }
    data.warpPoints.push_back(warpData);
}

static void readWarpSpawnData(const pugi::xml_node& spawnPointData, ReaderContext& context)
{
    auto& data = context.scene;
    WarpSpawnPointData warpData =
    {
        (spawnPointData.attribute("y").as_int() * SCALING_FACTOR) / 32,
        (spawnPointData.attribute("x").as_int() * SCALING_FACTOR) / 32,
        spawnPointData.child("properties").find_child_by_attribute("name", "id").attribute("value").as_int()
    };

    data.spawnPoints.push_back(warpData);
}

static void readCollisionData(const pugi::xml_node& collisionData, ReaderContext& context)
{
    auto& data = context.scene;
    data.collisions.push_back
    ({
        collisionData.attribute("x").as_int() * SCALING_FACTOR,
        collisionData.attribute("y").as_int() * SCALING_FACTOR,
        collisionData.attribute("width").as_int() * SCALING_FACTOR,
        collisionData.attribute("height").as_int() * SCALING_FACTOR
    });
}

static void readObjectData(const pugi::xml_node& root, ReaderContext& context)
{
    auto objectGroup = root.child("objectgroup");
    for (auto o : objectGroup.children())
    {
        auto type = std::string(o.attribute("type").value());

        if (type == "WARP_POINT") { readWarpPoint(o, context); }
        else if (type == "WARP_SPAWN") { readWarpSpawnData(o, context); }
        else { readCollisionData(o, context); }
    }
}

static void readSceneFile(const std::string& path, ReaderContext& context)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.c_str());

    if (result.status != pugi::status_ok)
    {
        std::cout << result.description() << ": " << path << std::endl;
    }
    else
    {
        auto root = doc.child("map");
        readLayerData(root, context);
        readObjectData(root, context);
    }
}

SceneData readSceneFile(const std::string& path)
{
    ReaderContext context;
    readSceneFile(path, context);
    return context.scene;
}