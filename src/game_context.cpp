#include <iostream>
#include <stdio.h>
#include "player_input_handler.h"
#include "player_movement.h"
#include "player_graphics.h"
#include "bucket_head_graphics.h"
#include "game_context.h"
#include "static_item_graphics.h"
#include "grid.h"
#include "interact_handler.h"
#include "types.h"

GameContext::GameContext()
{
    _graphics = new GraphicsContext("test", SCREEN_WIDTH, SCREEN_HEIGHT, "resources/");
    _keyboard = new KeyboardHandler();
    _player = getEntity(EntityType::PLAYER);
    _dialog = new Dialog(_graphics, _player);
}

GameContext::~GameContext()
{
    delete _keyboard;
    delete _graphics;
    delete _player;
    delete _dialog;
}

GraphicsContext* GameContext::getGraphics()
{
    return _graphics;
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
                NULL,
				0,
				0,
				34,
				29,
				Direction::DOWN
			);
            break;
        case EntityType::BUCKET_HEAD:
            e = new Entity
			(
				NULL,
				NULL,
				new BucketHeadGraphics(*_graphics, _player),
                new InteractHandler(this, "bucket_head/bucket_head.png", "i am the bucket"),
				300,
				5,
				34,
				26,
				Direction::DOWN
			);
            break;
		case EntityType::TRASH:
			e = new Entity
			(
				NULL,
				NULL,
				new StaticItemGraphics(_graphics->getTexture("trash.png")),
                new InteractHandler(this, "tim.png", "it trash"),
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
    if (e != NULL)
    {
        _entities.push_back(e);
    }
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

void GameContext::broadcast(EventType event, Entity& src)
{
    for (auto e : _entities)
    {
        e->onEvent(event, src);
    }
}

void GameContext::openDialog(const char* imagePath, const char* text)
{
    _dialog->open(imagePath, text);
}

void GameContext::hideEntities()
{
    for (auto e : _entities)
    {
        if (e->getId() != 0)
        {
            e->setVisibility(false);
        }
    }
}

void GameContext::showEntities()
{
    for (auto e : _entities)
    {
        if (e->getId() != 0)
        {
            e->setVisibility(true);
        }
    }
}

void GameContext::run()
{
    auto renderer = _graphics->getRenderer();
    auto trash = getEntity(EntityType::TRASH);
    auto trash2 = getEntity(EntityType::TRASH);
    auto bucketHead = getEntity(EntityType::BUCKET_HEAD);
    trash2->setX(250);
    trash2->setY(220);
    Grid grid(*_graphics);
    SDL_Event windowEvent;
    Uint32 before = 0;
    Uint32 after = 0;
    Uint32 wait = 0;
    Uint32 rate = 1000 / GraphicsContext::FRAME_RATE;
    bool showFrameRate = false;
    Uint32 framesRendered = 0;
    float lastTime = 0;
    while (true)
    {
        float currentTime = ((float)SDL_GetTicks()) / 1000;
        float timeStep = currentTime - lastTime;
        lastTime = currentTime;
        before = SDL_GetTicks();
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
            {
                break;
            }
            if (windowEvent.type == SDL_KEYDOWN || windowEvent.type == SDL_KEYUP)
            {
                _keyboard->handleEvent(windowEvent.key);
                if (windowEvent.key.keysym.sym == SDLK_h)
                {
                    hideEntities();
                }

                if (windowEvent.key.keysym.sym == SDLK_s)
                {
                    showEntities();
                }

                if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_r)
                {
                    showFrameRate = !showFrameRate;
                }
            }
        }

        if (!_dialog->isOpen())
        {
            _player->processInput(*_keyboard);
        }
        else
        {
            _dialog->processInput(*_keyboard);
        }
        _player->update(timeStep);
        grid.draw(renderer);
        trash->draw(renderer);
        trash2->draw(renderer);
        bucketHead->draw(renderer);
        _player->draw(renderer);
        _dialog->draw(renderer);
        if (showFrameRate)
        {
            SDL_Rect out = {0, 0, 60, 30};
            char frameRateBuffer[4];
            int frameRate = framesRendered / (before / 1000);
            sprintf(frameRateBuffer, "%d", frameRate);
            SDL_Texture* texture = _graphics->getFontTexture(frameRateBuffer);
            SDL_RenderCopy(renderer, texture, NULL, &out);
            SDL_DestroyTexture(texture);
        }
        SDL_RenderPresent(renderer);
        after = SDL_GetTicks();
        wait = after - before;

        SDL_Delay(1000 / 120);//rate - (wait > rate ? 0 : wait));

        framesRendered++;
        before = 0;
        after = 0;
    }
}
