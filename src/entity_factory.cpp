#include "entity_factory.h"
#include "game_context.h"
#include "player_input_handler.h"
#include "player_movement.h"
#include "player_graphics.h"
#include "bucket_head_graphics.h"
#include "found_item_interact_handler.h"
#include "simple_text_interact_handler.h"
#include "static_item_graphics_factory.h"
#include "trash_interact_handler.h"

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
                [](GameContext& c) {},
				0,
				0,
				17,
				29,
				Direction::DOWN,
                type,
                true,
                false
			));
            break;
        case EntityType::BUCKET_HEAD:
            e = std::make_shared<Entity>(Entity
			(
				nullptr,
				nullptr,
				BucketHeadGraphics::getInstance(_context->getGraphics(), _context->getPlayer().get()),
                new SimpleTextInteractHandler(_context, "bucket_head/bucket_head.png", "i am the bucket"),
                [](GameContext& c) {},
				350,
				230,
				34,
				26,
				Direction::DOWN,
                type,
                true,
                false
			));
            break;
		case EntityType::TRASH:
			e = std::make_shared<Entity>(Entity
			(
				nullptr,
				nullptr,
				StaticItemGraphicsFactory::getGraphics(_context->getGraphics(), type),
                TrashInteractHandler::getInstance(_context),
                [](GameContext& c) {},
				250,
				220,
				32,
				32,
				Direction::DOWN,
                type,
                true,
                false
			));
            break;
        case EntityType::LONELY_TOWN_SIGN:
            e = std::make_shared<Entity>(Entity
            (
                nullptr,
                nullptr,
                StaticItemGraphicsFactory::getGraphics(_context->getGraphics(), type),
                nullptr,
                [](GameContext& c) {},
                8 * 32,
                2*32,
                64,
                96,
                Direction::DOWN,
                type,
                false,
                true
            ));
            break;
        case EntityType::NEWSPAPER_RACK:
            e = std::make_shared<Entity>(Entity(
                nullptr,
                nullptr,
                StaticItemGraphicsFactory::getGraphics(_context->getGraphics(), type),
                new FoundItemInteractHandler(_context),
                [](GameContext& c) {},
                32,
                96,
                32,
                32,
                Direction::DOWN,
                type,
                true,
                false
            ));
            break;
        case EntityType::WARP_POINT:
            e = std::make_shared<Entity>(Entity(
                nullptr,
                nullptr,
                StaticItemGraphicsFactory::getGraphics(_context->getGraphics(), type),
                nullptr,
                [](GameContext& c) {
                    c.loadScene(getSceneData(Scenes::LONELY_TOWN_OUTSKIRTS_BUILDING));
                    c.getPlayer()->setX(9 * 32);
                    c.getPlayer()->setY(9 * 32);
                    c.getAudio().playSound("audio/door.ogg");
                },
                13 * 32,
                5 * 32,
                32,
                32,
                Direction::DOWN,
                type,
                true,
                false
            ));
            break;
		default:
            break;
	}

    return e;
}