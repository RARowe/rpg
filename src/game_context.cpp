#include <iostream>
#include <memory>
#include "backgrounds.h"
#include "game_context.h"
#include "frame_rate.h"
#include "script_steps/dialogue_step.h"
#include "script_steps/modify_entities_step.h"
#include "empty_graphics.h"
#include "scenes.h"
#include "time_step.h"

static void normalStateHandler(GameContext& context)
{
    KeyboardHandler& k = context.getKeyboardHandler();
    
    if (k.isPressedAndConsume(SDLK_RETURN))
    {
        context.openMenu(MenuType::PAUSE);
    }
    else
    {
        context.getPlayer()->processInput(k);
    }
}

static void pauseMenuStateHandler(GameContext& context)
{
    KeyboardHandler& k = context.getKeyboardHandler();
    MenuManager& m = context.getMenuManager();

    if (k.isPressedAndConsume(SDLK_RETURN))
    {
        context.closeAllMenus();
    }
    else
    {
        if (k.isPressedAndConsume(SDLK_UP))
        {
            m.doAction(MenuAction::MOVE_CURSOR_UP);
        }
        else if (k.isPressedAndConsume(SDLK_DOWN))
        {
            m.doAction(MenuAction::MOVE_CURSOR_DOWN);
        }
        else if (k.isPressedAndConsume(SDLK_LEFT))
        {
            m.doAction(MenuAction::MOVE_CURSOR_LEFT);
        }
        else if (k.isPressedAndConsume(SDLK_RIGHT))
        {
            m.doAction(MenuAction::MOVE_CURSOR_RIGHT);
        }

        if (k.isPressedAndConsume(SDLK_f))
        {
            m.doAction(MenuAction::CURSOR_CLICK);
        }

        if (k.isPressedAndConsume(SDLK_d))
        {
            m.closeCurrentMenu();
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
    _scene = new Scene(this);
    _menuManager = MenuManager::getInstance(this);
    _dialog = new TextBox(_graphics, _player.get(), _menuManager);
    _inputState = normalStateHandler;
}

GameContext::~GameContext()
{
    delete _keyboard;
    delete _graphics;
    delete _scene;
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

MenuManager& GameContext::getMenuManager()
{
    return *_menuManager;
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

void GameContext::addInteraction(const InteractData& interactData)
{
    std::shared_ptr<Entity> e = _entityFactory->getInteraction(interactData);

    if (e) { _entities.push_back(e); }
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
    setInputState(InputState::MENU);
    _dialog->open(imagePath, text);
    _menuManager->open(_dialog);
}

void GameContext::openTextBox(TileSets t, int tile, const std::string& text)
{
    setInputState(InputState::MENU);
    _dialog->open(t, tile, text);
    _menuManager->open(_dialog);
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
        case InputState::MENU:
            _inputState = pauseMenuStateHandler;
            break;
        // case InputState::SCRIPT_RUNNING:
        //     _inputState = scriptStateHandler;
        //     break;
        default:
            _inputState = normalStateHandler;
            break;
    }
}

void GameContext::toggleFrameRate()
{
    _showFrameRate = !_showFrameRate;
}



void GameContext::openMenu(MenuType type)
{
    if (type == MenuType::PAUSE)
    {
        _audio.playPauseMenuMusic(true);
    }
    _menuManager->open(type);
    setInputState(InputState::MENU);
}

void GameContext::onAllMenusClosed()
{
    _audio.playPauseMenuMusic(false);
    setInputState(InputState::NORMAL);
}

void GameContext::closeAllMenus()
{
    _menuManager->closeAllMenus();
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
    TimeStep timeStep;
    SDL_Event event;
    float lastTime = 0;
    _audio.play("audio/back_pocket.wav");
    _scene->load(getSceneData(Scenes::LONELY_TOWN_OUTSKIRTS));
    while (true)
    {
        float currentTime = ((float)SDL_GetTicks()) / 1000;
        float localTimeStep = currentTime - lastTime;
        timeStep.setTimeStep(localTimeStep);
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

        _player->tick(localTimeStep);
        _player->update(localTimeStep);
        _scene->draw(*_graphics, localTimeStep);
        _menuManager->draw(timeStep);
        if (_showFrameRate)
        {
            frameRate.draw(localTimeStep);
        }
        _graphics->present();
        SDL_Delay(1000 / GraphicsContext::FRAME_RATE);
    }
}
