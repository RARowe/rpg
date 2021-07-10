#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "scene.h"
#include "game_context.h"
#include "game_math.h"
#include "inventory.h"

Scene::Scene(GameContext* context) : _context(context) { }

void Scene::load(SceneData* data)
{
    _sceneData = data;
    
    _backgroundData = data->background;
    _midgroundData = data->midground;
    _foregroundData = data->foreground;
    _tileSet = data->tileSet;
    _context->clearEntities();
    _timeSinceLastSpawn = 0.0f;
    _nextSpawnTime = (float)(std::rand() % 15);
    // TODO(SCENE): This could be better
    _context->sceneData = data;
}

void Scene::update(const float timeStep)
{
    for (auto&& pair : _sceneData->enemySpawnZones) {
        auto&& zone = pair.second;
        if (zone.currentNumberOfEnemies < zone.maxNumberOfEnemies) {
            _timeSinceLastSpawn += timeStep;
        }
        if (_timeSinceLastSpawn > _nextSpawnTime)
        {
            _context->addEnemy();
            _timeSinceLastSpawn = 0.0f;
            _nextSpawnTime = (float)(std::rand() % 15);
            // TODO(ENEMIES): This does not reset on scene change
            zone.currentNumberOfEnemies++;
        }
    }
}

void drawPlayer(GameContext* context, Entity& e, const TimeStep timeStep)
{
    GraphicsContext* g = context->graphics;
    Direction d = e.direction;
    // if is moving
    if (e->vel.xVel != 0 || e->vel.yVel != 0)
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

void Scene::draw(GraphicsContext& graphics, const TimeStep timeStep)
{

    graphics.drawTiles(_tileSet, _backgroundData);
    graphics.drawTiles(_tileSet, _midgroundData);
    drawPlayer(context, *context->player, timeStep);
    for (auto&& p : _sceneData->tileSprites) {
        auto&& body = _sceneData->gameEntities[p.first];
        graphics.drawTile(_tileSet, p.second, body.x, body.y, body.w, body.h);
    }
    for (auto&& w : _sceneData->warpPoints) {
        auto&& body = _sceneData->gameEntities[w.first];
        graphics.drawTile(TileSets::OUTDOOR, (int)SpriteSheetTexture::WOODEN_DOOR_ROUNDED_WINDOW_CLOSED, body.x, body.y, body.w, body.h);
    }
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
        case EntityType::CD:
            g->drawTile(
                    TileSets::OUTDOOR,
                    (int)ItemSheetTexture::CD,
                    e.body.x,
                    e.body.y,
                    e.body.w,
                    e.body.h);
            break;
        case EntityType::ENEMY:
            // TODO: This needs to flash on appear again
            g->drawTexture(e, "enemy.png");
        default:
            break;
    }
    g->drawHitbox(e.body.x, e.body.y, e.body.w, e.body.h);
}

