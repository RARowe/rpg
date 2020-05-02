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
        std::shared_ptr<Entity> getEntity(EntityType t);
        std::shared_ptr<Entity> getInteraction(const InteractData& interactData);
        std::shared_ptr<Entity> getWarpPoint(const WarpPointData& warpData);
    private:
        EntityFactory(GameContext* context) : _context(context) { }
        GameContext* _context;
};
#endif