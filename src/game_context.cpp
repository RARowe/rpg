#include "player_input_handler.h"
#include "player_movement.h"
#include "player_graphics.h"
#include "game_context.h"
#include "static_item_graphics.h"

GameContext::GameContext()
{
    _graphics = new GraphicsContext("test", SCREEN_WIDTH, SCREEN_HEIGHT, "resources/");
    _interactor = new Interactor();
    _keyboard = new KeyboardHandler();
    //_collisionDetector = new CollisionDetector();
}

GameContext::~GameContext()
{
    delete _keyboard;
    delete _interactor;
    delete _graphics;
    //delete _collisionDetector;
}

GraphicsContext* GameContext::getGraphics()
{
    return _graphics;
}

Interactor* GameContext::getInteractor()
{
    return _interactor;
}

KeyboardHandler* GameContext::getKeyboardHandler()
{
    return _keyboard;
}

//CollisionDetector* GameContext::getCollisionDetector()
//{
//    return _collisionDetector;
//}

Entity* GameContext::getEntity(EntityType type)
{
    Entity* e;
	switch (type)
	{
		case EntityType::PLAYER:
            e = new Entity
			(
				new PlayerInputHandler(),
				new PlayerMovement(this),
				new PlayerGraphics(*_graphics),
				0,
				0,
				34,
				29,
				Direction::DOWN
			);
            break;
		case EntityType::TRASH:
			e = new Entity
			(
				NULL,
				NULL,
				new StaticItemGraphics(_graphics->getTexture("trash.png")),
				200,
				100,
				32,
				32,
				Direction::DOWN
			);
            break;
		default:
			e = NULL;
            break;
	}
    _entities.push_back(e);
    return e;
}

bool GameContext::isCollision(const Entity& e) const
{
    for (auto e2 : _entities)
    {
        if (e.collidesWith(*e2))
        {
            return true;
        }
    }

    return false;
}
