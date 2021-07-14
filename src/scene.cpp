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
    // TODO(SCENE): This could be better
    _context->sceneData = data;
}

void Scene::toggleBackground() {
    _drawBackground = !_drawBackground;
}

void Scene::toggleMidground() {
    _drawMidground = !_drawMidground;
}

void Scene::toggleForeground() {
    _drawForeground = !_drawForeground;
}

void Scene::draw(GraphicsContext& graphics, const float timeStep)
{

    if (_drawBackground) { graphics.drawTiles(_tileSet, _backgroundData); }
    else { graphics.drawBox(0, 0, 1000, 1000, Color::BLACK, 255); }
    if (_drawMidground) { graphics.drawTiles(_tileSet, _midgroundData); }
    for (auto&& p : _sceneData->tileSprites) {
        auto&& body = _sceneData->bodies[p.first];
        graphics.drawTile(_tileSet, p.second, body.x, body.y, body.w, body.h);
    }
    for (auto&& w : _sceneData->warpPoints) {
        auto&& body = _sceneData->bodies[w.first];
        graphics.drawTile(TileSets::OUTDOOR, (int)SpriteSheetTexture::WOODEN_DOOR_ROUNDED_WINDOW_CLOSED, body.x, body.y, body.w, body.h);
    }
    if (_drawForeground) { graphics.drawTiles(_tileSet, _foregroundData); }
}
