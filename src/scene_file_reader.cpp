#include "scene_file_reader.h"
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include "pugixml.hpp"
#include "types.h"
#define SCALING_FACTOR 2

class TemplateCache
{
    public:
        void init(const std::string workingPath)
        {
            _workingPath = workingPath;
        }
        const pugi::xml_node operator [](const std::string& path)
        {
            if (_templates.count(path) == 0)
            {
                pugi::xml_document doc;
                _templates[path] = std::make_unique<pugi::xml_document>();
                auto newPath = _workingPath + path;
                auto result = _templates[path]->load_file(newPath.c_str());

                if (result.status != pugi::status_ok)
                {
                    std::cout << result.description() << ": " << path << std::endl;
                }
            }

            return _templates[path]->child("template").child("object");
        }
    private:
        std::map<std::string, std::unique_ptr<pugi::xml_document>> _templates;
        std::string _workingPath;
};

class TiledObjectReader
{
    public:
        void init(const std::string path)
        {
            _cache.init(path);
        }
        int readAttributeInt(const pugi::xml_node& node, const pugi::char_t* name)
        {
            return getAttribute(node, name).as_int();
        }
        const pugi::char_t* readAttributeString(const pugi::xml_node& node, const pugi::char_t* name)
        {
            return getAttribute(node, name).as_string();
        }
        int readPropertyInt(const pugi::xml_node& node, const pugi::char_t* name)
        {
            return getPropertyAttribute(node, name).as_int();
        }
        std::string readPropertyString(const pugi::xml_node& node, const pugi::char_t* name){
            return getPropertyAttribute(node, name).as_string();
        }
    private:
        TemplateCache _cache;
        pugi::xml_node getParent(const pugi::xml_node& node)
        {
            pugi::xml_attribute templatePath;

            if ((templatePath = node.attribute("template")))
            {
                return _cache[templatePath.value()];
            }
            else
            {
                return pugi::xml_node();
            }
        }
        pugi::xml_attribute getAttribute(const pugi::xml_node& node, const pugi::char_t* name)
        {
            pugi::xml_node parent;

            if ((parent = getParent(node)))
            {
                pugi::xml_attribute attr;

                if ((attr = node.attribute(name)))
                {
                    return attr;
                }
                else
                {
                    return parent.attribute(name);
                }
            }
            else
            {
                return node.attribute(name);
            }
        }
        pugi::xml_attribute getPropertyAttribute(const pugi::xml_node& node, const pugi::char_t* name)
        {
            pugi::xml_node parent;

            if ((parent = getParent(node)))
            {
                pugi::xml_attribute attr;

                if ((attr = node.child("properties").find_child_by_attribute("name", name).attribute("value")))
                {
                    return attr;
                }
                else
                {
                    return parent.child("properties").find_child_by_attribute("name", name).attribute("value");
                }
            }
            else
            {
                return node.child("properties").find_child_by_attribute("name", name).attribute("value");
            }
        }
};

typedef struct ReaderContext
{
    SceneData scene;
    TiledObjectReader reader;
} ReaderContext;

static void readLayerCSVData(const std::string& csvData, std::vector<int>& container);
static void readLayerData(const pugi::xml_node& root, ReaderContext& context);
static void readSceneFile(const std::string& path, ReaderContext& context);
static void readObjectData(const pugi::xml_node& root, ReaderContext& context);
static void readWarpPoint(const pugi::xml_node& warpPointData, ReaderContext& context);
static void readWarpSpawnData(const pugi::xml_node& spawnPointData, ReaderContext& context);
static void readCollisionData(const pugi::xml_node& collisionData, ReaderContext& context);
static void readEntities(const pugi::xml_node& entityData, ReaderContext& context);
static void readWalls(const pugi::xml_node& entityData, ReaderContext& context);

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
    auto& reader = context.reader;
    WarpPointData warpData;

    warpData.row = (reader.readAttributeInt(warpPointData, "y") * SCALING_FACTOR) / 32;
    warpData.column = (reader.readAttributeInt(warpPointData, "x") * SCALING_FACTOR) / 32;
    warpData.destinationWarpSpawn = reader.readPropertyInt(warpPointData, "destination_warp_spawn");
    warpData.sceneToLoad = (Scenes)reader.readPropertyInt(warpPointData, "scene");
    warpData.audio = "audio/door.ogg";

    context.scene.warpPoints.push_back(warpData);
}

static void readWarpSpawnData(const pugi::xml_node& spawnPointData, ReaderContext& context)
{
    auto& reader = context.reader;

    context.scene.spawnPoints.push_back
    ({
        (reader.readAttributeInt(spawnPointData, "y") * SCALING_FACTOR) / 32,
        (reader.readAttributeInt(spawnPointData, "x") * SCALING_FACTOR) / 32,
        reader.readPropertyInt(spawnPointData, "id")
    });
}

static void readCollisionData(const pugi::xml_node& collisionData, ReaderContext& context)
{
    auto& reader = context.reader;
    auto& data = context.scene;
    data.collisions.push_back
    ({
        reader.readAttributeInt(collisionData, "x") * SCALING_FACTOR,
        reader.readAttributeInt(collisionData, "y") * SCALING_FACTOR,
        reader.readAttributeInt(collisionData, "width") * SCALING_FACTOR,
        reader.readAttributeInt(collisionData, "height") * SCALING_FACTOR
    });
}

static void readEntities(const pugi::xml_node& root, ReaderContext& context)
{
    auto objectGroup = root.find_child_by_attribute("name", "entities");

    Body b;
    for (auto&& o : objectGroup.children())
    {
        int entityId = context.scene.gameEntities.size();
        b.x = context.reader.readAttributeInt(o, "x") * SCALING_FACTOR;
        b.y = context.reader.readAttributeInt(o, "y") * SCALING_FACTOR;
        b.w = context.reader.readAttributeInt(o, "width") * SCALING_FACTOR;
        b.h = context.reader.readAttributeInt(o, "height") * SCALING_FACTOR;

        if (o.attribute("gid")) {
            context.scene.tileSprites[entityId] = context.reader.readAttributeInt(o, "gid") - 1;
        }
        context.scene.gameEntities.push_back(b);

        for (auto&& property : o.child("properties").children()) {
            auto&& name = std::string(context.reader.readAttributeString(property, "name"));
            if (name == "SOLID" && std::string(context.reader.readAttributeString(property, "value")) == "true") {
                context.scene.solidEntities.insert(entityId);
            } else if (name == "TEXT_INTERACTION") {
                context.scene.textInteractions[entityId] = std::string(context.reader.readAttributeString(property, "value"));
            }
        }
    }
}

static void readWalls(const pugi::xml_node& root, ReaderContext& context) {
    auto objectGroup = root.find_child_by_attribute("name", "walls");

    Body b;
    for (auto&& o : objectGroup.children())
    {
        int entityId = context.scene.gameEntities.size();
        b.x = context.reader.readAttributeInt(o, "x") * SCALING_FACTOR;
        b.y = context.reader.readAttributeInt(o, "y") * SCALING_FACTOR;
        b.w = context.reader.readAttributeInt(o, "width") * SCALING_FACTOR;
        b.h = context.reader.readAttributeInt(o, "height") * SCALING_FACTOR;

        context.scene.gameEntities.push_back(b);

        context.scene.solidEntities.insert(entityId);
    }
}

static void readObjectData(const pugi::xml_node& root, ReaderContext& context)
{
    auto objectGroup = root.find_child_by_attribute("name", "objects");
    for (auto o : objectGroup.children())
    {
        auto type = std::string(context.reader.readAttributeString(o, "type"));

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
        readEntities(root, context);
        readWalls(root, context);
    }
}

SceneData readSceneFile(const std::string& path, const std::string& fileName)
{
    ReaderContext context;
    context.reader.init(path);
    readSceneFile(path + fileName, context);
    return context.scene;
}
