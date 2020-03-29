#include <iostream>
#include "background_cache.h"
#include "backgrounds.h"
#include "player_input_handler.h"
#include "player_movement.h"
#include "player_graphics.h"
#include "bucket_head_graphics.h"
#include "game_context.h"
#include "static_item_graphics.h"
#include "simple_text_interact_handler.h"
#include "frame_rate.h"
#include "script_steps/dialog_step.h"
#include "trash_interact_handler.h"

GameContext::GameContext()
{
    _graphics = new GraphicsContext("test", SCREEN_WIDTH, SCREEN_HEIGHT, "resources/");
    _keyboard = new KeyboardHandler();
    _player = getEntity(EntityType::PLAYER);
    _dialog = new Dialog(_graphics, _player);
	_cache = new BackgroundCache("resources/backgrounds");
    _grid = new Grid(*_graphics, _cache);
}

GameContext::~GameContext()
{
    delete _keyboard;
    delete _graphics;
    delete _player;
    delete _dialog;
	delete _cache;
	delete _grid;
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
                new SimpleTextInteractHandler(this, "bucket_head/bucket_head.png", "i am the bucket"),
				410,
				180,
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
                new TrashInteractHandler(this),
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

void GameContext::runScript(ScriptType script)
{
    switch (script)
    {
        case ScriptType::ITS_JUST_TRASH:
            const char* bucket = "bucket_head/bucket_head.png";
            _scriptRunner
                .addStep(new DialogStep(this, bucket, "Hey! Stop that!"))
                .addStep(new DialogStep(this, bucket, "It's just trash."))
                .addStep(new DialogStep(this, "tim.png", "..."))
                .addStep(new DialogStep(this, bucket, "Just leave it alone."))
                .addStep(new DialogStep(this, bucket, "You're freaking me out."))
                .addStep(new DialogStep(this, bucket, "You keep staring into the void."))
                .addStep(new DialogStep(this, bucket, "But the void is full of gum wrappers."))
                .addStep(new DialogStep(this, "tim.png", "But... mayhaps..."))
                .addStep(new DialogStep(this, bucket, "No. Make better use of your time."));
            _scriptRunner.run();
            break;
    }
}

void GameContext::run()
{
    auto renderer = _graphics->getRenderer();
    auto trash = getEntity(EntityType::TRASH);
    auto trash2 = getEntity(EntityType::TRASH);
    auto bucketHead = getEntity(EntityType::BUCKET_HEAD);
    FrameRate frameRate(_graphics);
    trash2->setX(250);
    trash2->setY(220);
    _grid->load("field");
    SDL_Event windowEvent;
    bool showFrameRate = false;
    float lastTime = 0;
    while (true)
    {
        float currentTime = ((float)SDL_GetTicks()) / 1000;
        float timeStep = currentTime - lastTime;
        lastTime = currentTime;
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
            if (!_scriptRunner.isRunning())
            {
                _player->processInput(*_keyboard);
            }
            else
            {
                _scriptRunner.processStep();
            }
        }
        else
        {
            _dialog->processInput(*_keyboard);
        }
        _player->update(timeStep);
        _grid->draw(renderer);
        trash->draw(renderer);
        trash2->draw(renderer);
        bucketHead->draw(renderer);
        _player->draw(renderer);
        _dialog->draw(renderer);
        if (showFrameRate)
        {
            frameRate.draw(timeStep);
        }
        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / GraphicsContext::FRAME_RATE);
    }
}
