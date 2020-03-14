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
    _player = getEntity(EntityType::PLAYER);
    _dialog = new Dialog(_player, _graphics->getTexture("text_box.png"));
}

GameContext::~GameContext()
{
    delete _keyboard;
    delete _interactor;
    delete _graphics;
    delete _player;
    delete _dialog;
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
    _dialog->open("tim.png", "it trash...");
}

void GameContext::run()
{
    auto renderer = _graphics->getRenderer();
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

        _player->processInput(*_keyboard);
        _player->update();
        grid.draw(renderer);
		trash->draw(renderer);
        _player->draw(renderer);
        _dialog->draw(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / GraphicsContext::FRAME_RATE);
    }
}
