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
        case EntityType::BUCKET_HEAD: return BucketHeadGraphics::getInstance(context->getGraphics(), context->getPlayer().get());
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

std::shared_ptr<Entity> EntityFactory::getWarpPoint(const WarpPointData& warpData)
{
    return std::make_shared<Entity>(Entity(
        EntityType::WARP_POINT,
        nullptr,
        nullptr,
        StaticItemGraphicsFactory::getGraphics(_context->getGraphics(), EntityType::WARP_POINT),
        nullptr,
        [warpData](GameContext& c, Entity& e1, Entity& e2) {
            if (e2.id != c.getPlayer()->id) { return; }
            c.loadScene(warpData.sceneToLoad, warpData.destinationWarpSpawn);
            c.getAudio().playSound(warpData.audio);
        },
        warpData.column * 32,
        warpData.row * 32,
        32,
        32
    ));
}

std::shared_ptr<Entity> EntityFactory::getWarpSpawnPoint(const WarpSpawnPointData& data)
{
    auto e = std::make_shared<Entity>(Entity(
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
    ));
    e->isCollidable = false;
    return e;
}

std::shared_ptr<Entity> EntityFactory::getCollidable(const CollisionData& data)
{
    return std::make_shared<Entity>(Entity(
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
    ));
}

std::shared_ptr<Entity> EntityFactory::getInteraction(const InteractData& interactData)
{
    return std::make_shared<Entity>(Entity(
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
    ));
}

std::shared_ptr<Entity> EntityFactory::getEnemy()
{
    int column = std::rand() % 19;
    int row = std::rand() % 13;
    auto e = std::make_shared<Entity>(Entity(
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
    ));
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

    return e;
}

std::shared_ptr<Entity> EntityFactory::getEntity(EntityType type)
{
    std::shared_ptr<Entity> e;
	switch (type)
	{
		case EntityType::PLAYER:
            e = getEntity(type, 9, 9, 28, 32);
            break;
        case EntityType::BUCKET_HEAD:
            e = getEntity(type, 7, 11, 26, 34);
            break;
		case EntityType::TRASH:
            e = getEntity(type, 7, 8, 32, 32);
            break;
        case EntityType::LONELY_TOWN_SIGN:
            e = getEntity(type, 2, 8, 96, 64);
            e->isCollidable = false;
            e->isInForeground = true;
            break;
		default:
            break;
	}

    return e;
}

std::shared_ptr<Entity> EntityFactory::getEntity(EntityType t, int row, int column, int width, int height)
{
    return std::make_shared<Entity>(Entity(
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
    ));
}
