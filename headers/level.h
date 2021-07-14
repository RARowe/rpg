#ifndef LEVEL_H
#define LEVEL_H
#include <map>
#include <memory>
#include <vector>
#include "enums.h"
#include "scene.h"

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
        void draw(const float timeStep);
        Scene* getScene();
    private:
        GameContext* _context;
        Scene* _scene;
        std::map<Scenes, SceneData> _scenes;
};
#endif
