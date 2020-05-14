#include "scene_file_reader.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
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
inline void readSceneFile(const std::string& path, SceneData& data);
inline void readObjectData(const pugi::xml_node& root, SceneData& data);
inline void readWarpPoint(const pugi::xml_node& warpPointData, SceneData& data);
inline void readWarpSpawnData(const pugi::xml_node& spawnPointData, SceneData& data);
inline void readCollisionData(const pugi::xml_node& collisionData, SceneData& data);

inline void readLayerCSVData(const std::string& csvData, std::vector<int>& container)
{
    std::stringstream stream(csvData);
    std::string value;
    while (std::getline(stream, value, ','))
    {container.push_back(std::stoi(value) - 1);
        // if (value[0] != '\n')
        // {
        //     container.push_back(std::stoi(value) - 1);
        // }
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

inline void readSceneFile(const std::string& path, SceneData& data)
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
        readLayerData(root, data);
        readObjectData(root, data);
        std::cout << data << std::endl;
    }
}

SceneData readSceneFile(const std::string& path)
{
    SceneData data;
    readSceneFile(path, data);
    return data;
}