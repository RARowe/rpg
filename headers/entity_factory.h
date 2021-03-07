#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H
#include <memory>
#include "entity.h"
#include "enums.h"
#include "scenes.h"

class GameContext;

class EntityFactory
{
    public:
        static EntityFactory* getInstance(GameContext* context)
        {
            static EntityFactory _factory(context);
            return &_factory;
        }
        void initEntity(Entity* e, EntityType t);
        void initEntity(Entity* e, EntityType t, int row, int column, int width, int height);
        void initInteraction(Entity* e, const InteractData& interactData);
        void initEnemy(Entity* e);
    private:
        EntityFactory(GameContext* context) : _context(context) { }
        GameContext* _context;
};
#endif
