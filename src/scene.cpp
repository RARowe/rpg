#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "scene.h"
#include "game_context.h"
#include "game_math.h"
#include "inventory.h"

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

void drawEntities(
        GameContext* context,
        const TimeStep timeStep
        );

void Scene::draw(GraphicsContext& graphics, const TimeStep timeStep)
{
    graphics.drawTiles(_tileSet, _backgroundData);
    graphics.drawTiles(_tileSet, _midgroundData);
    drawEntities(_context, timeStep);
    graphics.drawTiles(_tileSet, _foregroundData);
}

// TODO: This should be somewhere else
// TODO: Graphics system should be more flexible than this
void drawBucketHead(GameContext* context, Entity& e, const TimeStep timeStep) {
    GraphicsContext* graphics = context->graphics;
    int sprite = 0;
    if ((BucketHeadStateType)e.state == BucketHeadStateType::NORMAL)
    {
        float distance = distanceBetween(e, *context->player);
        if (distance > 150.0f)
        {
            sprite = 0;
        }
        else if (distance > 100.0f)
        {
            sprite = 1;
        }
        else if (distance > 50.0f)
        {
            sprite = 2;
        }
        else
        {
            sprite = (((int)(timeStep.getTotalTime() / 0.5)) % 2) + 3;
        }
    }
    else
    {
        sprite = (((int)(timeStep.getTotalTime() / 0.5)) % 2) + 3;
    }
    graphics->drawSprite("bucket_head", sprite, e);

    if (e.isEmoting)
    {
        graphics->drawEmote(e, "");
    }
}

void drawEntity(GameContext* context, Entity& e, const TimeStep timeStep) {
    GraphicsContext* g = context->graphics;
    switch (e.type) {
        case EntityType::BUCKET_HEAD:
            drawBucketHead(context, e, timeStep);
            break;
        case EntityType::TRASH:
            g->drawTile(TileSets::OUTDOOR, (int)SpriteSheetTexture::TRASH, e.body.x, e.body.y, e.body.w, e.body.h);
            break;
        case EntityType::LONELY_TOWN_SIGN:
            g->drawTexture(e, "lonely_town_sign.png");
            break;
        case EntityType::CD:
            g->drawTile(
                    TileSets::OUTDOOR,
                    (int)ItemSheetTexture::CD,
                    e.body.x,
                    e.body.y,
                    e.body.w,
                    e.body.h);
            break;
        case EntityType::WARP_POINT:
            g->drawTile(TileSets::OUTDOOR, (int)SpriteSheetTexture::WOODEN_DOOR_ROUNDED_WINDOW_CLOSED, e.body.x, e.body.y, e.body.w, e.body.h);
            break;
        case EntityType::ENEMY:
            // TODO: This needs to flash on appear again
            g->drawTexture(e, "enemy.png");
        case EntityType::OBJECT_TILE:
        default:
            break;
    }
    g->drawHitbox(e.body.x, e.body.y, e.body.w, e.body.h);
}

// TODO: Probably a bad place for this, but this is the only place it's used
static bool isMoving(Entity* e) { return e->vel.xVel != 0 || e->vel.yVel != 0; }

void drawPlayer(GameContext* context, Entity& e, const TimeStep timeStep)
{
    GraphicsContext* g = context->graphics;
    Direction d = e.direction;
    if (isMoving(&e))
    {
        g->drawWalkingSprite(timeStep, d, "theo", e);
    }
    else
    {
        g->drawStandingSprite(d, "theo", e);
    }

    if (e.state == (int)PlayerStateType::ITEM_FOUND)
    {
        g->drawAbove(e, TileSets::ITEMS, (int)inventory_get_item_texture(context->inventory->mostRecentlyAdded));
    }
}

void drawEntities
(
    GameContext* context,
    const TimeStep timeStep
) {
    for (short i = 1; i < context->entities.back; i++) {
        Entity& e = context->entities.entities[i];
        if (!e.isInForeground)
        {
            drawEntity(context, e, timeStep);
        }
    }
    drawPlayer(context, *context->player, timeStep);
    for (short i = 1; i < context->entities.back; i++) {
        Entity& e = context->entities.entities[i];
        if (e.isInForeground)
        {
            drawEntity(context, e, timeStep);
        }
    }
}
