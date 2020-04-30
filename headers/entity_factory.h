#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H
#include <memory>
#include "entity.h"
#include "enums.h"

class GameContext;

class EntityFactory
{
    public:
        static EntityFactory* getInstance(GameContext* context)
        {
            static EntityFactory _factory(context);
            return &_factory;
        }
        std::shared_ptr<Entity> getEntity(EntityType t);
    private:
        EntityFactory(GameContext* context) : _context(context) { }
        GameContext* _context;
};
#endif