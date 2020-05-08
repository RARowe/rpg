#include <cstdio>
#include <filesystem>
#include <fstream>
#include "scene.h"
#include "game_context.h"

Scene::Scene(GameContext* context) : _context(context) { }

void Scene::load
(
    const SceneData& data,
    const std::vector<int> background,
    const std::vector<int> objects,
    const std::vector<int> foreground
)
{
    auto entities = data.entities;
    
    _backgroundData = background;
    _objectData = objects;
    _foregroundData = foreground;
    _tileSet = data.tileSet;
    _context->clearEntities();
    _context->loadObjectLayerCollisionDetection(_objectData);
    for (auto e : entities)
    {
        _context->addEntity(e);
    }
    for (auto i : data.interactions)
    {
        _context->addInteraction(i);
    }
    for (auto w : data.warpPoints)
    {
        _context->addWarpPoint(w);
    }
    _numberOfEnemies = 0;
    _maxNumberOfEnemies = data.maxNumberOfEnemies;
    _timeSinceLastSpawn = 0.0f;
    _nextSpawnTime = (float)(std::rand() % 15);
}

void Scene::update(const float timeStep)
{
    if (_numberOfEnemies < _maxNumberOfEnemies)
    {
        _timeSinceLastSpawn += timeStep;
        if (_timeSinceLastSpawn > _nextSpawnTime)
        {
            _context->addEnemy();
            _timeSinceLastSpawn = 0.0f;
            _nextSpawnTime = (float)(std::rand() % 15);
            _numberOfEnemies++;
        }
    }
}

void Scene::draw(GraphicsContext& graphics, float timeStep)
{
    graphics.drawTiles(_tileSet, _backgroundData);
    graphics.drawTiles(_tileSet, _objectData);
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
    graphics.drawTiles(_tileSet, _foregroundData);
}