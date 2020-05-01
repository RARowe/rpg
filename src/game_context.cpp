#include <iostream>
#include <memory>
#include "background_cache.h"
#include "backgrounds.h"
#include "game_context.h"
#include "frame_rate.h"
#include "script_steps/dialogue_step.h"
#include "script_steps/modify_entities_step.h"
#include "empty_graphics.h"
#include "scenes.h"

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
    PauseMenu& p = context.getPauseMenu();

    if (k.isPressedAndConsume(SDLK_RETURN))
    {
        context.pause();
    }
    else
    {
        if (k.isPressedAndConsume(SDLK_DOWN))
        {
            p.cursorDown();
        }
        else if (k.isPressedAndConsume(SDLK_UP))
        {
            p.cursorUp();
        }

        if (k.isPressedAndConsume(SDLK_f))
        {
            p.click();
        }
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
    _entityFactory = EntityFactory::getInstance(this);
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

PauseMenu& GameContext::getPauseMenu()
{
    return *_pauseMenu;
}

bool GameContext::gameEventHasHappened(GameEvent event)
{
    return _gameEvents.count(event) > 0;
}

void GameContext::clearEntities()
{
    _entities.clear();
    _entities.push_back(_player);
}

void GameContext::broadcastGameEvent(GameEvent event)
{
    _gameEvents.insert(event);
}

void GameContext::addEntity(EntityType type)
{
    std::shared_ptr<Entity> e = _entityFactory->getEntity(type);
    if (e)
    {
        _entities.push_back(e);
    }
}

void GameContext::addWarpPoint(const WarpPointData& warpPoint)
{
    std::shared_ptr<Entity> e = _entityFactory->getWarpPoint(warpPoint);

    if (e) { _entities.push_back(e); }
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
            e2->onCollision(*this);
        }
    }
}

void GameContext::broadcast(EventType event, Entity& src)
{
    if (event == EventType::CHANGE_SCENE)
    {
        if (_showScene)
        {
            _scene->load(getSceneData(Scenes::LONELY_TOWN_OUTSKIRTS));
        }
        else
        {
            _scene->load(getSceneData(Scenes::LONELY_TOWN_ENTRANCE));
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
                [](GameContext& c) {},
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

void GameContext::loadScene(const SceneData& scene)
{
    _scene->load(scene);
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
    FrameRate frameRate(_graphics);
    SDL_Event event;
    float lastTime = 0;
    _audio.play("audio/back_pocket.wav");
    _scene->load(getSceneData(Scenes::LONELY_TOWN_OUTSKIRTS));
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
        _dialog->draw();
        _pauseMenu->draw();
        if (_showFrameRate)
        {
            frameRate.draw(timeStep);
        }
        _graphics->present();
        SDL_Delay(1000 / GraphicsContext::FRAME_RATE);
    }
}
