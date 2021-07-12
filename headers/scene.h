#pragma once
#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include <string>
#include <vector>
#include "enums.h"
#include "graphics_context.h"

class GameContext;

class Scene
{
    public:
        Scene(GameContext* context);
        void load(SceneData* data);
        void draw(GraphicsContext& graphics, const float timeStep);
    private:
        SceneData* _sceneData;
        GameContext* _context;
        std::vector<int> _backgroundData;
        std::vector<int> _midgroundData;
        std::vector<int> _foregroundData;
        TileSets _tileSet = TileSets::OUTDOOR;
};
#endif
