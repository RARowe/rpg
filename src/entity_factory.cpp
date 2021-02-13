#include "entity_factory.h"
#include "game_context.h"
#include "player_input_handler.h"
#include "player_movement.h"
#include "player_graphics.h"
#include "empty_graphics.h"
#include "enemy_movement.h"
#include "enemy_graphics.h"
#include "bucket_head_graphics.h"
#include "found_item_interact_handler.h"
#include "simple_text_interact_handler.h"
#include "static_item_graphics_factory.h"
#include "trash_interact_handler.h"

static InputHandler* getInputHandler(GameContext* context, EntityType type)
{
    switch (type)
    {
        case EntityType::PLAYER: return PlayerInputHandler::getInstance(context);
        default: return nullptr;
    }
}

static MovementHandler* getMovementHandler(GameContext* context, EntityType type)
{
    switch (type)
    {
        case EntityType::PLAYER: return PlayerMovement::getInstance(context);
        default: return nullptr;
    }
}

static GraphicsHandler* getGraphicsHandler(GameContext* context, EntityType type)
{
    switch (type)
    {
        case EntityType::PLAYER: return PlayerGraphics::getInstance(context->getGraphics());
        case EntityType::BUCKET_HEAD: return BucketHeadGraphics::getInstance(context->getGraphics(), context->player);
        default: return StaticItemGraphicsFactory::getGraphics(context->getGraphics(), type);
    }
}

static InteractHandler* getInteractHandler(GameContext* context, EntityType type)
{
    switch (type)
    {
        case EntityType::BUCKET_HEAD: return new SimpleTextInteractHandler(context, "bucket_head/bucket_head.png", "i am the bucket");
        case EntityType::TRASH: return TrashInteractHandler::getInstance(context);
        default: return nullptr;
    }
}

void EntityFactory::initWarpPoint(Entity* e, const WarpPointData& warpData)
{
    Entity::initEntity(
        e,
        EntityType::WARP_POINT,
        nullptr,
        nullptr,
        StaticItemGraphicsFactory::getGraphics(_context->getGraphics(), EntityType::WARP_POINT),
        nullptr,
        [warpData](GameContext& c, Entity& e1, Entity& e2) {
            if (e2.id != c.player->id) { return; }
            c.loadScene(warpData.sceneToLoad, warpData.destinationWarpSpawn);
            c.getAudio().playSound(warpData.audio);
        },
        warpData.column * 32,
        warpData.row * 32,
        32,
        32
    );
}

void EntityFactory::initWarpSpawnPoint(Entity* e, const WarpSpawnPointData& data)
{
    Entity::initEntity(
        e,
        EntityType::WARP_SPAWN_POINT,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        [](GameContext& c, Entity&, Entity&) {},
        data.column * 32,
        data.row * 32,
        32,
        32
    );
    e->isCollidable = false;
}

void EntityFactory::initCollidable(Entity* e, const CollisionData& data)
{
    Entity::initEntity(
        e,
        EntityType::OBJECT_TILE,
        nullptr,
        nullptr,
        EmptyGraphics::shared_instance(_context->getGraphics()),
        nullptr,
        [](GameContext& c, Entity&, Entity&) {},
        data.x,
        data.y,
        data.w,
        data.h
    );
}

void EntityFactory::initInteraction(Entity* e, const InteractData& interactData)
{
    Entity::initEntity(
        e,
        EntityType::INTERACTION,
        nullptr,
        nullptr,
        nullptr,
        new FoundItemInteractHandler(_context, interactData.interactHandler),
        [](GameContext& c, Entity&, Entity&) {},
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
        EnemyMovement::getInstance(_context),
        EnemyGraphics::shared_instance(_context->getGraphics()),
        nullptr,
        [](GameContext& c, Entity&, Entity&) {},
        32 * column,
        32 * row,
        32,
        32
    );
    e->maxVelocity = 100.0f;

    while (_context->isCollision(*e))
    {
        column++;
        if (column > 18)
        {
            column = 0;
            row++;
        }
        e->pos.x = column * 32;
        e->pos.y = row * 32;
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
		case EntityType::TRASH:
            initEntity(e, type, 7, 8, 32, 32);
            break;
        case EntityType::LONELY_TOWN_SIGN:
            initEntity(e, type, 2, 8, 96, 64);
            e->isCollidable = false;
            e->isInForeground = true;
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
        getInputHandler(_context, t),
        getMovementHandler(_context, t),
        getGraphicsHandler(_context, t),
        getInteractHandler(_context, t),
        [](GameContext& c, Entity&, Entity&) {},
        column * 32,
        row * 32,
        width,
        height
    );
}
