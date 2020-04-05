#include <iostream>

#include <cstdio>
#include <filesystem>
#include <fstream>

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

GameContext::GameContext()
{
    _graphics = new GraphicsContext("test", SCREEN_WIDTH, SCREEN_HEIGHT, "resources/");
    _keyboard = new KeyboardHandler();
    _player = getEntity(EntityType::PLAYER);
    _dialog = new TextBox(_graphics, _player);
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

TextBox& GameContext::getTextBox()
{
    return *_dialog;
}

std::vector<Entity*>& GameContext::getEntities()
{
    return _entities;
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
				new PlayerGraphics(_graphics),
                NULL,
				0,
				0,
				34,
				29,
				Direction::DOWN,
                type
			);
            break;
        case EntityType::BUCKET_HEAD:
            e = new Entity
			(
				NULL,
				NULL,
				new BucketHeadGraphics(_graphics, _player),
                new SimpleTextInteractHandler(this, "bucket_head/bucket_head.png", "i am the bucket"),
				350,
				230,
				34,
				26,
				Direction::DOWN,
                type
			);
            break;
		case EntityType::TRASH:
			e = new Entity
			(
				NULL,
				NULL,
				new StaticItemGraphics(_graphics, "trash.png"),
                new TrashInteractHandler(this),
				200,
				100,
				32,
				32,
				Direction::DOWN,
                type
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
            _scriptRunner
                .addStep(new ModifyEntitiesStep(*this, true))
                .addStep(new DialogueStep(this))
                .addStep(new ModifyEntitiesStep(*this, false));
            _scriptRunner.run();
            break;
    }
}

static std::vector<int> readBackgroundCSVFile(const std::string& path)
{
    std::vector<int> ret;
   	std::string line;
   	std::ifstream infile(path);
    const char* format = "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d";
    int o[19];
   	if (infile)
    {
   	   	while (std::getline(infile, line))
        {
            for (int i = 0; i < line.size(); i++)
            {
                std::sscanf
                (
                    line.c_str(),
                    format,
                    &o[0], &o[1], &o[2],
                    &o[3], &o[4], &o[5],
                    &o[6], &o[7], &o[8],
                    &o[9], &o[10], &o[11],
                    &o[12], &o[13], &o[14],
                    &o[15], &o[16], &o[17], &o[18]);
            }

            for (auto i : o)
            {
                ret.push_back(i);
            }
   	   	}
   	}
   	infile.close();

    return ret;
}

void GameContext::run()
{
    auto renderer = _graphics->getRenderer();
    auto trash = getEntity(EntityType::TRASH);
    auto bucketHead = getEntity(EntityType::BUCKET_HEAD);
    FrameRate frameRate(_graphics);
    trash->setX(250);
    trash->setY(220);
    _grid->load("field");
    SDL_Event windowEvent;
    bool showFrameRate = false;
    float lastTime = 0;
    bool song = true;
    _audio.play("audio/back_pocket.wav");
    auto backgroundInfo = readBackgroundCSVFile("resources/backgrounds/lonely_town/entrance/background.csv");
    auto objectInfo = readBackgroundCSVFile("resources/backgrounds/lonely_town/entrance/objects.csv");
    auto foregroundInfo = readBackgroundCSVFile("resources/backgrounds/lonely_town/entrance/foreground.csv");
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
        _graphics->drawTiles("tiles.png", backgroundInfo, 13, 19);//_grid->draw(renderer);
        trash->draw(timeStep);
        bucketHead->draw(timeStep);
        _player->draw(timeStep);
        _graphics->drawTiles("tiles.png", objectInfo, 13, 19);
        _graphics->drawTiles("tiles.png", foregroundInfo, 13, 19);
        _dialog->draw(renderer);
        if (showFrameRate)
        {
            frameRate.draw(timeStep);
        }
        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / GraphicsContext::FRAME_RATE);
    }
}
