#include <iostream>
#include <memory>
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
#include "script_steps/dialogue_step.h"
#include "script_steps/modify_entities_step.h"
#include "trash_interact_handler.h"
#include "empty_graphics.h"
#include "static_item_graphics_factory.h"

GameContext::GameContext()
{
    _graphics = new GraphicsContext("test", SCREEN_WIDTH, SCREEN_HEIGHT, "resources/");
    _keyboard = new KeyboardHandler();
    addEntity(EntityType::PLAYER);
    _player = _entities[0];
    _dialog = new TextBox(_graphics, _player.get());
	_cache = new BackgroundCache("resources/backgrounds");
    _grid = new Grid(*_graphics, _cache);
    _scene = new Scene(this);
}

GameContext::~GameContext()
{
    delete _keyboard;
    delete _graphics;
    delete _dialog;
	delete _cache;
	delete _grid;
    delete _scene;
}

GraphicsContext* GameContext::getGraphics()
{
    return _graphics;
}

KeyboardHandler* GameContext::getKeyboardHandler()
{
    return _keyboard;
}

TextBox& GameContext::getTextBox()
{
    return *_dialog;
}

std::shared_ptr<Entity> GameContext::getPlayer()
{
    return _player;
}

std::vector<std::shared_ptr<Entity>>& GameContext::getEntities()
{
    return _entities;
}

void GameContext::clearEntities()
{
    _entities.clear();
    _entities.push_back(_player);
}

void GameContext::addEntity(EntityType type)
{
    std::shared_ptr<Entity> e;
	switch (type)
	{
		case EntityType::PLAYER:
            e = std::make_shared<Entity>(Entity
			(
				PlayerInputHandler::getInstance(this),
				PlayerMovement::getInstance(this),
				PlayerGraphics::getInstance(_graphics),
                nullptr,
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
				BucketHeadGraphics::getInstance(_graphics, _player.get()),
                new SimpleTextInteractHandler(this, "bucket_head/bucket_head.png", "i am the bucket"),
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
				StaticItemGraphicsFactory::getGraphics(_graphics, type),
                TrashInteractHandler::getInstance(this),
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
                StaticItemGraphicsFactory::getGraphics(_graphics, type),
                nullptr,
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
        case EntityType::NEWSPAPER_KIOSK:
            e = std::make_shared<Entity>(Entity(
                nullptr,
                nullptr,
                StaticItemGraphicsFactory::getGraphics(_graphics, type),
                nullptr,
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
		default:
            break;
	}
    if (e)
    {
        _entities.push_back(e);
    }
}

void GameContext::resolveCollision(Entity& e, int oldX, int oldY)
{
    for (auto e2 : _entities)
    {
        if (e2->isCollidable() && e.collidesWith(*e2))
        {
            int currentX = e.getX();
            e.setX(oldX);
            if (e.collidesWith(*e2))
            {
                e.setX(currentX);
                e.setY(oldY);
                if (e.collidesWith(*e2))
                {
                    e.setX(oldX);
                }
            }
        }
    }
}

void GameContext::broadcast(EventType event, Entity& src)
{
    if (event == EventType::CHANGE_SCENE)
    {
        if (_showScene)
        {
            auto types = std::vector<EntityType> { EntityType::NEWSPAPER_KIOSK };
            _scene->load("resources/backgrounds/lonely_town/outskirts", types);
        }
        else
        {
            auto types = std::vector<EntityType> { EntityType::LONELY_TOWN_SIGN, EntityType::TRASH, EntityType::BUCKET_HEAD };
            _scene->load("resources/backgrounds/lonely_town/entrance", types);
        }
        _showScene = !_showScene;
    }
    else
    {
        for (auto e : _entities)
        {
            e->onEvent(event, src);
        }
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
            _scriptRunner
                .addStep(new ModifyEntitiesStep(*this, true))
                .addStep(new DialogueStep(this))
                .addStep(new ModifyEntitiesStep(*this, false));
            _scriptRunner.run();
            break;
    }
}

void GameContext::loadObjectLayerCollisionDetection(const std::vector<int>& objectLayer)
{
    int x = 0,
        y = 0,
        row = 0,
        column = 0;
    for (auto i : objectLayer)
    {
        x = column * 32;
        y = row * 32;
        if (i > -1)
        {
            _entities.push_back(std::make_shared<Entity>(Entity(
                nullptr,
                nullptr,
                EmptyGraphics::shared_instance(_graphics),
                nullptr,
                x,
                y,
                32,
                32,
                Direction::DOWN,
                EntityType::OBJECT_TILE,
                true,
                false
            )));
        }
        column++;
        if (column == 19)
        {
            column = 0;
            row++;
        }
    }
}

void GameContext::toggleHitboxView()
{
    _graphics->toggleHitboxView();
}

void GameContext::run()
{
    auto renderer = _graphics->getRenderer();
    FrameRate frameRate(_graphics);
    SDL_Event windowEvent;
    bool showFrameRate = false;
    float lastTime = 0;
    bool song = true;
    _audio.play("audio/back_pocket.wav");
    auto types = std::vector<EntityType> { };
    _scene->load("resources/backgrounds/lonely_town/outskirts", types);
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

                if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_b)
                {
                    toggleHitboxView();
                }

                if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_m)
                {
                    if (song)
                    {
                        _audio.play("audio/pause_menu_song.wav");
                    }
                    else
                    {
                        _audio.play("audio/back_pocket.wav");
                    }
                    song = !song;
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
        _scene->draw(*_graphics, timeStep);
        _dialog->draw(renderer);
        if (showFrameRate)
        {
            frameRate.draw(timeStep);
        }
        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / GraphicsContext::FRAME_RATE);
    }
}
