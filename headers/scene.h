#pragma once
#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include <string>
#include <vector>
#include "graphics_context.h"

class GameContext;

class Scene
{
    public:
        Scene(std::shared_ptr<GameContext> context);
        void load(const std::string& path);
        void draw(GraphicsContext& graphics, float timeStep);
    private:
        std::shared_ptr<GameContext> _context;
        std::vector<int> _backgroundData;
        std::vector<int> _objectData;
        std::vector<int> _foregroundData;
};
#endif