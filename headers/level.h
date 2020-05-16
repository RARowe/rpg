#ifndef LEVEL_H
#define LEVEL_H
#include <map>
#include <memory>
#include <vector>
#include "enums.h"
#include "scene.h"
#include "scenes.h"
#include "time_step.h"

class GameContext;

typedef struct LevelData
{
    std::string name;
    std::vector<Scenes> scenes;
} LevelData;

class Level
{
    public:
        Level(GameContext* context);
        void load(Levels level);
        void load(Scenes scene);
        void load(Scenes scene, int spawnId);
        void update(const float timeStep);
        void draw(const TimeStep timeStep);
    private:
        GameContext* _context;
        std::unique_ptr<Scene> _scene;
        std::map<Scenes, SceneData> _scenes;
};
#endif