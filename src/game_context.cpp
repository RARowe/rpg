#include "player_input_handler.h"
#include "player_movement.h"
#include "player_graphics.h"
#include "game_context.h"
#include "static_item_graphics.h"
#include "grid.h"

GameContext::GameContext()
{
    _graphics = new GraphicsContext("test", SCREEN_WIDTH, SCREEN_HEIGHT, "resources/");
    _interactor = new Interactor();
    _keyboard = new KeyboardHandler();
    //_collisionDetector = new CollisionDetector();
    _dialogOpen = false;
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
				new PlayerInputHandler(this),
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

void GameContext::interact(int x, int y)
{
    openDialog("tim.png", "it trash...");
}

void GameContext::openDialog(const char* imagePath, const char* text)
{
    _dialogOpen = !_dialogOpen;
}

void GameContext::run()
{
    auto renderer = _graphics->getRenderer();
    auto dialogTexture = _graphics->getTexture("text_box.png");
    SDL_Rect dialogRect = {1, 250, 608, 150};
	auto player = getEntity(EntityType::PLAYER);
	auto trash = getEntity(EntityType::TRASH);
    Grid grid(*_graphics);
    SDL_Event windowEvent;
    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
            {
                break;
            }
            if (windowEvent.type == SDL_KEYDOWN || windowEvent.type == SDL_KEYUP)
            {
                _keyboard->handleEvent(windowEvent.key);
            }
        }

        player->processInput(*_keyboard);
        player->update();
        grid.draw(renderer);
		trash->draw(renderer);
        player->draw(renderer);
        if (_dialogOpen)
        {
            int playerY = player->getY();
            if (playerY > 250)
            {
                dialogRect.y = 1;
            }
            else if (dialogRect.y == 1 && playerY < 150)
            {
                dialogRect.y = 250;
            }
            SDL_RenderCopy(renderer, dialogTexture, NULL, &dialogRect);
        }
        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / GraphicsContext::FRAME_RATE);
    }
}
