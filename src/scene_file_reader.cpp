#include "scene_file_reader.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string.h>
#include <nlohmann/json.hpp>
#include "pugixml.hpp"
#define SCALING_FACTOR 2

std::ostream& operator << (std::ostream& o, const std::vector<WarpPointData>& ws)
{
    o << "Warp point data:" << std::endl;
    for (auto w : ws)
    {
        o << "row: " << w.row << std::endl;
        o << "col: " << w.column << std::endl;
        o << "scene: " << (int)w.sceneToLoad << std::endl;
        o << "destination: " << w.destinationWarpSpawn << std::endl;
    }
    return o;
}

std::ostream& operator << (std::ostream& o, const std::vector<WarpSpawnPointData>& ws)
{
    o << "Warp spawn point data:" << std::endl;
    for (auto w : ws)
    {
        o << "row: " << w.row << std::endl;
        o << "col: " << w.column << std::endl;
        o << "id: " << w.id << std::endl;
    }
    return o;
}

std::ostream& operator << (std::ostream& o, const std::vector<CollisionData>& cs)
{
    o << "Collision data:" << std::endl;
    for (auto c : cs)
    {
        o << "x: " << c.x << std::endl;
        o << "y: " << c.y << std::endl;
        o << "w: " << c.w << std::endl;
        o << "h: " << c.h << std::endl;
    }
    return o;
}

std::ostream& operator << (std::ostream& o, const std::vector<int>& tileData)
{
    o << "Tile data: " << std::endl;
    for (auto i : tileData)
    {
        o << i << " ";
    }
    o << std::endl;
    return o;
}

std::ostream& operator << (std::ostream& o, const SceneData& s)
{
    o << s.warpPoints << std::endl;
    o << s.spawnPoints << std::endl;
    o << s.collisions << std::endl;
    o << s.background << std::endl;
    o << s.midground << std::endl;
    o << s.foreground << std::endl;
    return o;
}

inline void readLayerCSVData(const std::string& csvData, std::vector<int>& container);
inline void readLayerData(const pugi::xml_node& root, SceneData& data);
inline void readSceneFile(const std::string& path);
inline void readObjectData(const pugi::xml_node& root, SceneData& data);
inline void readWarpPoint(const pugi::xml_node& warpPointData, SceneData& data);
inline void readWarpSpawnData(const pugi::xml_node& spawnPointData, SceneData& data);
inline void readCollisionData(const pugi::xml_node& collisionData, SceneData& data);

inline void readLayerCSVData(const std::string& csvData, std::vector<int>& container)
{
    std::stringstream stream(csvData);
    std::string value;
    while (std::getline(stream, value, ','))
    {
        if (value[0] != '\n')
        {
            container.push_back(std::stoi(value));
        }
    }
}

inline void readLayerData(const pugi::xml_node& root, SceneData& data)
{
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


inline void readWarpPoint(const pugi::xml_node& warpPointData, SceneData& data)
{
    WarpPointData warpData =
    {
        (warpPointData.attribute("y").as_int() * SCALING_FACTOR) / 32,
        (warpPointData.attribute("x").as_int() * SCALING_FACTOR) / 32
    };

    for (auto&& p : warpPointData.child("properties").children())
    {
        auto name = std::string(p.attribute("name").value());

        if (name == "destination_warp_spawn") { warpData.destinationWarpSpawn = p.attribute("value").as_int(); }
        else if (name == "scene") { warpData.sceneToLoad = (Scenes)p.attribute("value").as_int(); }
        else { std::cout << "Attribute name: '" << name << "' not recognized for warp point." << std::endl; }
    }
    data.warpPoints.push_back(warpData);
}

inline void readWarpSpawnData(const pugi::xml_node& spawnPointData, SceneData& data)
{
    WarpSpawnPointData warpData =
    {
        (spawnPointData.attribute("y").as_int() * SCALING_FACTOR) / 32,
        (spawnPointData.attribute("x").as_int() * SCALING_FACTOR) / 32,
        spawnPointData.child("properties").find_child_by_attribute("name", "id").attribute("value").as_int()
    };

    data.spawnPoints.push_back(warpData);
}

inline void readCollisionData(const pugi::xml_node& collisionData, SceneData& data)
{
    data.collisions.push_back
    ({
        collisionData.attribute("x").as_int() * SCALING_FACTOR,
        collisionData.attribute("y").as_int() * SCALING_FACTOR,
        collisionData.attribute("width").as_int() * SCALING_FACTOR,
        collisionData.attribute("height").as_int() * SCALING_FACTOR
    });
}

inline void readObjectData(const pugi::xml_node& root, SceneData& data)
{
    auto objectGroup = root.child("objectgroup");
    for (auto o : objectGroup.children())
    {
        auto type = std::string(o.attribute("type").value());

        if (type == "WARP_POINT") { readWarpPoint(o, data); }
        else if (type == "WARP_SPAWN") { readWarpSpawnData(o, data); }
        else { readCollisionData(o, data); }
    }
}

inline void readSceneFile(const std::string& path)
{
    SceneData data;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("asset_source_files/maps/lonely_town/outskirts.tmx");
    auto root = doc.child("map");
    
    readLayerData(root, data);
    readObjectData(root, data);
    std::cout << data << std::endl;
}

using json = nlohmann::json;

void readSceneFile
(
    const std::string& path,
    std::vector<int>& background,
    std::vector<int>& midground,
    std::vector<int>& foreground,
    std::vector<WarpPointData>& warpPoints,
    std::vector<WarpSpawnPointData>& warpSpawnsData,
    std::vector<CollisionData>& collisionData
)
{
    readSceneFile(" ");
    background.clear();
    midground.clear();
    foreground.clear();
    warpPoints.clear();
    warpSpawnsData.clear();
    collisionData.clear();
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
                else
                {
                    CollisionData c =
                    {
                        (int)object["x"] * 2,
                        (int)object["y"] * 2,
                        (int)object["width"] * 2,
                        (int)object["height"] * 2
                    };
                    collisionData.push_back(c);
                }
            }
        }
    }
}