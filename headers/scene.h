#pragma once
#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include <string>
#include <vector>
#include "enums.h"
#include "graphics_context.h"
#include "scenes.h"

class GameContext;

class Scene
{
    public:
        Scene(GameContext* context);
        void load
        (
            const SceneData& data,
            const std::vector<int>& background,
            const std::vector<int>& midground,
            const std::vector<int>& foreground,
            const std::vector<WarpPointData>& warpPoints
        );
        void update(const float timeStep);
        void draw(GraphicsContext& graphics, float timeStep);
    private:
        GameContext* _context;
        std::vector<int> _backgroundData;
        std::vector<int> _midgroundData;
        std::vector<int> _foregroundData;
        TileSets _tileSet = TileSets::OUTDOOR;
        int _numberOfEnemies = 0;
        int _maxNumberOfEnemies = 0;
        float _timeSinceLastSpawn = 0.0f;
        float _nextSpawnTime = 0.0f;
};
#endif