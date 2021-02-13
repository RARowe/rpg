#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "scene.h"
#include "game_context.h"

Scene::Scene(GameContext* context) : _context(context) { }

void Scene::load
(
    const SceneData& data,
    const std::vector<int>& background,
    const std::vector<int>& midground,
    const std::vector<int>& foreground,
    const std::vector<WarpPointData>& warpPoints,
    const std::vector<WarpSpawnPointData>& warpSpawns,
    const std::vector<CollisionData>& collisions
)
{
    auto entities = data.entities;
    
    _backgroundData = background;
    _midgroundData = midground;
    _foregroundData = foreground;
    _tileSet = data.tileSet;
    _context->clearEntities();
    //_context->loadObjectLayerCollisionDetection(_midgroundData);
    int times = 0;
    for (auto e : entities)
    {
        times++;
        _context->addEntity(e);
    }
    for (auto i : data.interactions)
    {
        times++;
        _context->addInteraction(i);
    }
    for (auto w : warpPoints)
    {
        times++;
        _context->addWarpPoint(w);
    }
    for (auto s : warpSpawns)
    {
        times++;
        _context->addWarpSpawnPoint(s);
    }
    for (auto c : collisions)
    {
        times++;
        _context->addCollidable(c);
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

void Scene::draw(GraphicsContext& graphics, const TimeStep timeStep)
{
    graphics.drawTiles(_tileSet, _backgroundData);
    graphics.drawTiles(_tileSet, _midgroundData);
    for (short i = 1; i < _context->entities.back; i++) {
        Entity& e = _context->entities.entities[i];
        if (!e.isInForeground)
        {
            e.draw(timeStep);
        }
    }
    _context->player->draw(timeStep);
    for (short i = 1; i < _context->entities.back; i++) {
        Entity& e = _context->entities.entities[i];
        if (e.isInForeground)
        {
            e.draw(timeStep);
        }
    }
    graphics.drawTiles(_tileSet, _foregroundData);
}
