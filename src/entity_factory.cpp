#include "entity_factory.h"
#include "game_context.h"
#include "found_item_interact_handler.h"
#include "simple_text_interact_handler.h"

static InteractHandler* getInteractHandler(GameContext* context, EntityType type)
{
    switch (type)
    {
        case EntityType::BUCKET_HEAD: return new SimpleTextInteractHandler(context, "bucket_head/bucket_head.png", "i am the bucket");
        default: return nullptr;
    }
}

void EntityFactory::initInteraction(Entity* e, const InteractData& interactData)
{
    Entity::initEntity(
        e,
        EntityType::INTERACTION,
        new FoundItemInteractHandler(_context, interactData.interactHandler),
        interactData.column * 32,
        interactData.row * 32,
        32,
        32
    );
}

void EntityFactory::initEnemy(Entity* e)
{
    int column = std::rand() % 19;
    int row = std::rand() % 13;
    Entity::initEntity(
        e,
        EntityType::ENEMY,
        nullptr,
        32 * column,
        32 * row,
        32,
        32
    );
    e->vel.maxVel = 100.0f;

    while (_context->isCollision(*e))
    {
        column++;
        if (column > 18)
        {
            column = 0;
            row++;
        }
        e->body.x = column * 32;
        e->body.y = row * 32;
    }
}

void EntityFactory::initEntity(Entity* e, EntityType type)
{
	switch (type)
	{
		case EntityType::PLAYER:
            initEntity(e, type, 9, 9, 28, 32);
            break;
        case EntityType::BUCKET_HEAD:
            initEntity(e, type, 7, 11, 26, 34);
            break;
		default:
            break;
	}
}

void EntityFactory::initEntity(Entity* e, EntityType t, int row, int column, int width, int height)
{
    Entity::initEntity(
        e, 
        t,
        getInteractHandler(_context, t),
        column * 32,
        row * 32,
        width,
        height
    );
}
