#ifndef LEVEL_H
#define LEVEL_H
#include <map>
#include <memory>
#include <vector>
#include "enums.h"
#include "scene.h"
#include "scenes.h"

class GameContext;

typedef struct LevelData
{
    std::string name;
    std::vector<Scenes> scenes;
} LevelData;

typedef struct TileData
{
    std::vector<int> background;
    std::vector<int> objects;
    std::vector<int> foreground;
    std::vector<WarpPointData> warpPoints;
    std::vector<WarpSpawnPointData> warpSpawns;
} TileData;

class Level
{
    public:
        Level(GameContext* context);
        void load(Levels level);
        void load(Scenes scene);
        void load(Scenes scene, int spawnId);
        void update(const float timeStep);
        void draw(const float timeStep);
    private:
        GameContext* _context;
        std::unique_ptr<Scene> _scene;
        std::map<Scenes, TileData> _scenes;
};
#endif