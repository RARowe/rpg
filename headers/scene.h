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
        void load(const std::string& path, const std::vector<EntityType>& entities);
        void draw(GraphicsContext& graphics, float timeStep);
    private:
        GameContext* _context;
        std::vector<int> _backgroundData;
        std::vector<int> _objectData;
        std::vector<int> _foregroundData;
};
#endif