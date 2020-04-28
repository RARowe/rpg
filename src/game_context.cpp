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
#include "found_item_interact_handler.h"

static void normalStateHandler(GameContext& context)
{
    KeyboardHandler& k = context.getKeyboardHandler();
    
    if (k.isPressedAndConsume(SDLK_RETURN))
    {
        context.pause();
    }
    else
    {
        context.getPlayer()->processInput(k);
    }
}

static void textBoxStateHandler(GameContext& context)
{
    TextBox& t = context.getTextBox();

    if (t.isOpen())
    {
        if (context.getKeyboardHandler().isPressedAndConsume(SDLK_f))
        {
            t.click();
        }
    }
    else
    {
        context.setInputState(InputState::NORMAL);
    }
}

static void pauseMenuStateHandler(GameContext& context)
{
    KeyboardHandler& k = context.getKeyboardHandler();
    
    if (k.isPressedAndConsume(SDLK_RETURN))
    {
        context.pause();
    }
}

static void scriptStateHandler(GameContext& context)
{
    TextBox& t = context.getTextBox();
    ScriptRunner& r = context.getScriptRunner();

    if (t.isOpen())
    {
        if (context.getKeyboardHandler().isPressedAndConsume(SDLK_f))
        {
            t.click();
        }
    }
    else
    {
        if (r.isRunning())
        {
            r.processStep();
        }
        else
        {
            context.setInputState(InputState::NORMAL);
        }
    }
}

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
    _pauseMenu = new PauseMenu(this);
    _inputState = normalStateHandler;
}

GameContext::~GameContext()
{
    delete _keyboard;
    delete _graphics;
    delete _dialog;
	delete _cache;
	delete _grid;
    delete _scene;
    delete _pauseMenu;
}

GraphicsContext* GameContext::getGraphics()
{
    return _graphics;
}

KeyboardHandler& GameContext::getKeyboardHandler()
{
    return *_keyboard;
}

TextBox& GameContext::getTextBox()
{
    return *_dialog;
}

Audio& GameContext::getAudio()
{
    return _audio;
}

std::shared_ptr<Entity> GameContext::getPlayer()
{
    return _player;
}

std::vector<std::shared_ptr<Entity>>& GameContext::getEntities()
{
    return _entities;
}

ScriptRunner& GameContext::getScriptRunner()
{
    return _scriptRunner;
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
        case EntityType::NEWSPAPER_RACK:
            e = std::make_shared<Entity>(Entity(
                nullptr,
                nullptr,
                StaticItemGraphicsFactory::getGraphics(_graphics, type),
                new FoundItemInteractHandler(this),
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
            auto types = std::vector<EntityType> { EntityType::NEWSPAPER_RACK };
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
    setInputState(InputState::TEXT_BOX);
    _dialog->open(imagePath, text);
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
            setInputState(InputState::SCRIPT_RUNNING);
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

void GameContext::setInputState(InputState s)
{
    switch (s)
    {
        case InputState::NORMAL:
            _inputState = normalStateHandler;
            break;
        case InputState::TEXT_BOX:
            _inputState = textBoxStateHandler;
            break;
        case InputState::PAUSE_MENU:
            _inputState = pauseMenuStateHandler;
            break;
        case InputState::SCRIPT_RUNNING:
            _inputState = scriptStateHandler;
            break;
        default:
            _inputState = normalStateHandler;
            break;
    }
}

void GameContext::toggleFrameRate()
{
    _showFrameRate = !_showFrameRate;
}

void GameContext::pause()
{
    _pauseMenu->open();
    
    if (_pauseMenu->isOpen())
    {
        setInputState(InputState::PAUSE_MENU);
    }
    else
    {
        setInputState(InputState::NORMAL);
    }
}

static void handleGlobalKeys(GameContext& context)
{
    KeyboardHandler& k = context.getKeyboardHandler();

    if (k.isPressedAndConsume(SDLK_r))
    {
        context.toggleFrameRate();
    }
    if (k.isPressedAndConsume(SDLK_b))
    {
        context.toggleHitboxView();
    }
}
void GameContext::run()
{
    auto renderer = _graphics->getRenderer();
    FrameRate frameRate(_graphics);
    SDL_Event event;
    float lastTime = 0;
    _audio.play("audio/back_pocket.wav");
    auto types = std::vector<EntityType> { };
    _scene->load("resources/backgrounds/lonely_town/outskirts", types);
    while (true)
    {
        float currentTime = ((float)SDL_GetTicks()) / 1000;
        float timeStep = currentTime - lastTime;
        lastTime = currentTime;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                break;
            }
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            {
                _keyboard->handleEvent(event.key);
            }
        }

        handleGlobalKeys(*this);
        _inputState(*this);

        _player->tick(timeStep);
        _player->update(timeStep);
        _scene->draw(*_graphics, timeStep);
        _dialog->draw(renderer);
        _pauseMenu->draw();
        if (_showFrameRate)
        {
            frameRate.draw(timeStep);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / GraphicsContext::FRAME_RATE);
    }
}
