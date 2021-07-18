#include "scene_file_reader.h"
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include "pugixml.hpp"
#include "types.h"
#include "enums.h"
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

static void readLayerCSVData(const std::string& csvData, int* container);
static void readLayerData(const pugi::xml_node& root, ReaderContext& context);
static void readSceneFile(const std::string& path, ReaderContext& context);
static void readObjectData(const pugi::xml_node& root, ReaderContext& context);
static void readWarpPoint(const pugi::xml_node& warpPointData, ReaderContext& context);
static void readSpawnPoint(const pugi::xml_node& spawnPointData, ReaderContext& context);
static void readEntities(const pugi::xml_node& entityData, ReaderContext& context);
static void readWalls(const pugi::xml_node& entityData, ReaderContext& context);

static void readLayerCSVData(const std::string& csvData, int* container)
{
    std::stringstream stream(csvData);
    std::string value;
    unsigned int i = 0;
    while (std::getline(stream, value, ','))
    {
        container[i] = std::stoi(value) - 1;
        i += 1;
    }
}

static void readLayerData(const pugi::xml_node& root, ReaderContext& context)
{
    auto& data = context.scene;
    // TODO(TILESET): We need a better way of handling this, taking filepath into account
    pugi::xml_node tileset = root.child("tileset");
    std::string tilesetName = std::string(tileset.attribute("source").value());

    if (tilesetName.find("outdoor") != std::string::npos) {
        data.tileSet = 0;
    } else {
        data.tileSet = 0;
    }

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

static Body readBody(const pugi::xml_node& node, ReaderContext& context) {
    auto& reader = context.reader;
    Body b;
    b.x = reader.readAttributeInt(node, "x") * SCALING_FACTOR;
    b.y = reader.readAttributeInt(node, "y") * SCALING_FACTOR;
    b.w = reader.readAttributeInt(node, "width") * SCALING_FACTOR;
    b.h = reader.readAttributeInt(node, "height") * SCALING_FACTOR;
    return b;
}
static void readWarpPoint(const pugi::xml_node& warpPointData, ReaderContext& context)
{
    auto& reader = context.reader;
    const int entityId = context.scene.bodies.size();

    WarpPoint w;
    w.destinationSpawn = reader.readPropertyInt(warpPointData, "destination_warp_spawn");
    w.sceneToLoad = (Scenes)reader.readPropertyInt(warpPointData, "scene");


    context.scene.bodies[entityId] = readBody(warpPointData, context);
    context.scene.warpPoints[entityId] = w;
    context.scene.solidEntities.insert(entityId);
}

static void readSpawnPoint(const pugi::xml_node& spawnPointData, ReaderContext& context)
{
    auto& reader = context.reader;
    const int entityId = context.scene.bodies.size();

    context.scene.bodies[entityId] = readBody(spawnPointData, context);
    const int spawnId = reader.readPropertyInt(spawnPointData, "id");
    context.scene.spawnPoints[spawnId] = entityId;
}

static void readEntities(const pugi::xml_node& root, ReaderContext& context)
{
    auto objectGroup = root.find_child_by_attribute("name", "entities");

    Body b;
    for (auto&& o : objectGroup.children())
    {
        int entityId = context.scene.bodies.size();
        b = readBody(o, context);

        if (o.attribute("gid")) {
            context.scene.tileSprites[entityId] = context.reader.readAttributeInt(o, "gid") - 1;
        }
        context.scene.bodies[entityId] = b;

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
        int entityId = context.scene.bodies.size();
        b = readBody(o, context);

        context.scene.bodies[entityId] = b;

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
        else if (type == "WARP_SPAWN") { readSpawnPoint(o, context); }
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
    // TODO: This is the "player"
    Body b = { 100, 100, 32, 32 };
    context.scene.bodies[0] = b;
    context.reader.init(path);
    readSceneFile(path + fileName, context);
    return context.scene;
}
