#include <cstdio>
#include <filesystem>
#include <fstream>
#include "scene.h"
#include "game_context.h"

static void readTileCSVFile(const std::string& path, std::vector<int>& fileDataOut)
{
   	std::string line;
   	std::ifstream infile(path);
    const char* format = "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d";
    int o[19];
    fileDataOut.clear();
   	if (infile)
    {
   	   	while (std::getline(infile, line))
        {
            for (int i = 0; i < line.size(); i++)
            {
                std::sscanf
                (
                    line.c_str(),
                    format,
                    &o[0], &o[1], &o[2],
                    &o[3], &o[4], &o[5],
                    &o[6], &o[7], &o[8],
                    &o[9], &o[10], &o[11],
                    &o[12], &o[13], &o[14],
                    &o[15], &o[16], &o[17], &o[18]);
            }

            for (auto i : o)
            {
                fileDataOut.push_back(i);
            }
   	   	}
   	}
   	infile.close();
}

Scene::Scene(GameContext* context) : _context(context)
{
}

void Scene::load(const SceneData& data)
{
    auto path = data.path;
    auto entities = data.entities;
    
    std::string backgroundPath = path + "/background.csv";
    std::string objectsPath = path + "/objects.csv";
    std::string foregroundPath = path + "/foreground.csv";
    readTileCSVFile(backgroundPath, _backgroundData);
    readTileCSVFile(objectsPath, _objectData);
    readTileCSVFile(foregroundPath, _foregroundData);
    _context->clearEntities();
    _context->loadObjectLayerCollisionDetection(_objectData);
    for (auto e : entities)
    {
        _context->addEntity(e);
    }
}

void Scene::draw(GraphicsContext& graphics, float timeStep)
{
    graphics.drawTiles("tiles.png", _backgroundData, 13, 19);
    graphics.drawTiles("tiles.png", _objectData, 13, 19);
    for (auto& e : _context->getEntities())
    {
        if (!e->isInForeground())
        {
            e->draw(timeStep);
        }
    }
    _context->getPlayer()->draw(timeStep);
    for (auto& e : _context->getEntities())
    {
        if (e->isInForeground())
        {
            e->draw(timeStep);
        }
    }
    graphics.drawTiles("tiles.png", _foregroundData, 13, 19);
}