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

std::shared_ptr<Entity> EntityFactory::getWarpPoint(const WarpPointData& warpData)
{
    return std::make_shared<Entity>(Entity(
        nullptr,
        nullptr,
        StaticItemGraphicsFactory::getGraphics(_context->getGraphics(), EntityType::WARP_POINT),
        nullptr,
        [warpData](GameContext& c, Entity& e1, Entity& e2) {
            if (e2.getId() != c.getPlayer()->getId()) { return; }
            c.loadScene(warpData.sceneToLoad, warpData.destinationWarpSpawn);
            c.getAudio().playSound(warpData.audio);
        },
        warpData.column * 32,
        warpData.row * 32,
        32,
        32,
        Direction::DOWN,
        EntityType::WARP_POINT,
        true,
        false,
        0.0f
    ));
}

std::shared_ptr<Entity> EntityFactory::getWarpSpawnPoint(const WarpSpawnPointData& data)
{
    return std::make_shared<Entity>(Entity(
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        [](GameContext& c, Entity&, Entity&) {},
        data.column * 32,
        data.row * 32,
        32,
        32,
        Direction::DOWN,
        EntityType::WARP_SPAWN_POINT,
        false,
        false,
        0.0f
    ));
}

std::shared_ptr<Entity> EntityFactory::getCollidable(const CollisionData& data)
{
    return std::make_shared<Entity>(Entity(
        nullptr,
        nullptr,
        EmptyGraphics::shared_instance(_context->getGraphics()),
        nullptr,
        [](GameContext& c, Entity&, Entity&) {},
        data.x,
        data.y,
        data.h,
        data.w,
        Direction::DOWN,
        EntityType::OBJECT_TILE,
        true,
        false,
        0.0f
    ));
}

std::shared_ptr<Entity> EntityFactory::getInteraction(const InteractData& interactData)
{
    return std::make_shared<Entity>(Entity(
        nullptr,
        nullptr,
        nullptr,
        new FoundItemInteractHandler(_context, interactData.interactHandler),
        [](GameContext& c, Entity&, Entity&) {},
        interactData.column * 32,
        interactData.row * 32,
        32,
        32,
        Direction::DOWN,
        EntityType::INTERACTION,
        true,
        false,
        0.0f
    ));
}

std::shared_ptr<Entity> EntityFactory::getEnemy()
{
    int column = std::rand() % 19;
    int row = std::rand() % 13;
    auto e = std::make_shared<Entity>(Entity(
        nullptr,
        EnemyMovement::getInstance(_context),
        EnemyGraphics::shared_instance(_context->getGraphics()),
        nullptr,
        [](GameContext& c, Entity&, Entity&) {},
        32 * column,
        32 * row,
        32,
        32,
        Direction::DOWN,
        EntityType::ENEMY,
        true,
        false,
        60.0f
    ));

    while (_context->isCollision(*e))
    {
        column++;
        if (column > 18)
        {
            column = 0;
            row++;
        }
        e->setX(column * 32);
        e->setY(row * 32);
    }

    return e;
}

std::shared_ptr<Entity> EntityFactory::getEntity(EntityType type)
{
    std::shared_ptr<Entity> e;
	switch (type)
	{
		case EntityType::PLAYER:
            e = std::make_shared<Entity>(Entity
			(
				PlayerInputHandler::getInstance(_context),
				PlayerMovement::getInstance(_context),
				PlayerGraphics::getInstance(_context->getGraphics()),
                nullptr,
                [](GameContext& c, Entity&, Entity&) {},
				9* 32,
				9 * 32,
				17,
				29,
				Direction::DOWN,
                type,
                true,
                false,
                0.0f
			));
            break;
        case EntityType::BUCKET_HEAD:
            e = std::make_shared<Entity>(Entity
			(
				nullptr,
				nullptr,
				BucketHeadGraphics::getInstance(_context->getGraphics(), _context->getPlayer().get()),
                new SimpleTextInteractHandler(_context, "bucket_head/bucket_head.png", "i am the bucket"),
                [](GameContext& c, Entity&, Entity&) {},
				350,
				230,
				34,
				26,
				Direction::DOWN,
                type,
                true,
                false,
                0.0f
			));
            break;
		case EntityType::TRASH:
			e = std::make_shared<Entity>(Entity
			(
				nullptr,
				nullptr,
				StaticItemGraphicsFactory::getGraphics(_context->getGraphics(), type),
                TrashInteractHandler::getInstance(_context),
                [](GameContext& c, Entity&, Entity&) {},
				250,
				220,
				32,
				32,
				Direction::DOWN,
                type,
                true,
                false,
                0.0f
			));
            break;
        case EntityType::LONELY_TOWN_SIGN:
            e = std::make_shared<Entity>(Entity
            (
                nullptr,
                nullptr,
                StaticItemGraphicsFactory::getGraphics(_context->getGraphics(), type),
                nullptr,
                [](GameContext& c, Entity&, Entity&) {},
                8 * 32,
                2*32,
                64,
                96,
                Direction::DOWN,
                type,
                false,
                true,
                0.0f
            ));
            break;
        case EntityType::NEWSPAPER_RACK:
            e = std::make_shared<Entity>(Entity(
                nullptr,
                nullptr,
                StaticItemGraphicsFactory::getGraphics(_context->getGraphics(), type),
                nullptr,
                [](GameContext& c, Entity&, Entity&) {},
                32,
                96,
                32,
                32,
                Direction::DOWN,
                type,
                true,
                false,
                0.0f
            ));
            break;
		default:
            break;
	}

    return e;
}