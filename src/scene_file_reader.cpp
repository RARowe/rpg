#include "scene_file_reader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "pugixml.hpp"

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

inline void readSceneFile(const std::string& path)
{
    SceneData data;
    std::cout << "attempting to read..." << std::endl;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("asset_source_files/maps/lonely_town/entrance.tmx");
    auto root = doc.child("map");
    
    readLayerData(root, data);
    for (auto i : data.background)
    {
        std::cout << i << std::endl;
    }
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