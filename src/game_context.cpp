#include <memory>
#include <stdlib.h>
#include "game_context.h"
#include "game_math.h"
#include "frame_rate.h"
#include "script_steps/dialogue_step.h"
#include "script_steps/modify_entities_step.h"
#include "scenes.h"
#include "time_step.h"
#include "levels.h"

static void normalStateHandler(GameContext& c)
{
    if (c.input.pause)
    {
        c.openMenu(MenuType::PAUSE);
    }
    else
    {
        c.player->processInput(c.input);
    }
}

static void textBoxStateHandler(GameContext& c)
{
    MenuManager& m = *c.menuManager;

    if (c.input.select)
    {
        m.doAction(MenuAction::CURSOR_CLICK);
    }
}

static void pauseMenuStateHandler(GameContext& c)
{
    MenuManager& m = *c.menuManager;

    if (c.input.pause)
    {
        c.closeAllMenus();
    }
    else
    {
        if (c.input.upClick)
        {
            m.doAction(MenuAction::MOVE_CURSOR_UP);
        }
        else if (c.input.downClick)
        {
            m.doAction(MenuAction::MOVE_CURSOR_DOWN);
        }
        else if (c.input.leftClick)
        {
            m.doAction(MenuAction::MOVE_CURSOR_LEFT);
        }
        else if (c.input.rightClick)
        {
            m.doAction(MenuAction::MOVE_CURSOR_RIGHT);
        }

        if (c.input.select)
        {
            m.doAction(MenuAction::CURSOR_CLICK);
        }

        if (c.input.back)
        {
            m.closeCurrentMenu();
        }
    }
}

GameContext::GameContext()
{
    entities.size = 500;
    entities.back = 0;
    entities.entities = (Entity*)calloc(500, sizeof(Entity) * 500);
    memset(&input, 0, sizeof(PlayerInput));
    graphics = new GraphicsContext("test", SCREEN_WIDTH, SCREEN_HEIGHT, "resources/");
    _entityFactory = EntityFactory::getInstance(this);
    addEntity(EntityType::PLAYER);
    player = &entities.entities[0];
    _level = new Level(this);
    menuManager = MenuManager::getInstance(this);
    dialog = new TextBox(graphics, player, menuManager);
    _gameState.push(InputState::NORMAL);
}

GameContext::~GameContext()
{
    delete graphics;
    delete _level;
}

bool GameContext::gameEventHasHappened(GameEvent event)
{
    return _gameEvents.count(event) > 0;
}

// TODO: Revisit this, I have a hunch memory zeroing may be slow
void GameContext::clearEntities()
{
    // Reset entities
    memset(&entities.entities[1], 0, sizeof(Entity) * entities.back);
    entities.back = 1;
}

void GameContext::broadcastGameEvent(GameEvent event)
{
    _gameEvents.insert(event);
}

void GameContext::addEntity(EntityType type)
{
    Entity* e = &entities.entities[entities.back];
    entities.back++;
    _entityFactory->initEntity(e, type);
}

void GameContext::addInteraction(const InteractData& interactData)
{
    Entity* e = &entities.entities[entities.back];
    entities.back++;
    _entityFactory->initInteraction(e, interactData);
}

void GameContext::addWarpPoint(const WarpPointData& warpPoint)
{
    Entity* e = &entities.entities[entities.back];
    entities.back++;
    _entityFactory->initWarpPoint(e, warpPoint);
}

void GameContext::addEnemy()
{
    Entity* e = &entities.entities[entities.back];
    entities.back++;
    _entityFactory->initEnemy(e);
}

void GameContext::addWarpSpawnPoint(const WarpSpawnPointData& data)
{
    Entity* e = &entities.entities[entities.back];
    entities.back++;
    _entityFactory->initWarpSpawnPoint(e, data);
}

void GameContext::addCollidable(const CollisionData& data)
{
    Entity* e = &entities.entities[entities.back];
    entities.back++;
    _entityFactory->initCollidable(e, data);
}

// TODO: this could be in a better place
bool entitiesCollide(const Entity& e1, const Entity& e2) {
	if (e1.id == e2.id) { return false; }
    int x2 = e1.pos.x+ e1.body.w,
        y2 = e1.pos.y + e1.body.h,
        e2x2 = e2.pos.x + e2.body.w,
        e2y2 = e2.pos.y + e2.body.h;
    bool below = e2.pos.y >= y2,
         above = e2y2 <= e1.pos.y,
         left = e2x2 <= e1.pos.x,
         right = e2.pos.x >= x2;
    return !(below || above || left || right);
}

bool GameContext::isCollision(const Entity& e)
{
    for (short i = 0; i < entities.back; i++) {
        Entity& e2 = entities.entities[i];
        if (e2.isCollidable && entitiesCollide(e, e2))
        {
            return true;
        }
    }
    return false;
}

void GameContext::resolveCollision(Entity& e, int oldX, int oldY)
{
    for (short i = 0; i < entities.back; i++) {
        Entity& e2 = entities.entities[i];
        if (e2.isCollidable && entitiesCollide(e, e2))
        {
            int currentX = e.pos.x;
            e.pos.x = oldX;
            if (entitiesCollide(e, e2))
            {
                e.pos.x = currentX;
                e.pos.y = oldY;
                if (entitiesCollide(e, e2))
                {
                    e.pos.x = oldX;
                }
            }
            e2.onCollision(*this, e);
        }
    }
}

void GameContext::broadcast(EventType event, Entity& src)
{
    if (event == EventType::CHANGE_SCENE)
    {
        if (_showScene)
        {
            _level->load(Scenes::LONELY_TOWN_OUTSKIRTS);
        }
        else
        {
            _level->load(Scenes::LONELY_TOWN_ENTRANCE);
        }
        _showScene = !_showScene;
    }
    else
    {
        for (short i = 0; i < entities.back; i++) {
            entities.entities[i].onEvent(event, src);
        }
    }
}

void GameContext::openDialog(const char* imagePath, const char* text)
{
    if (_gameState.top() == InputState::NORMAL)
    {
        setInputState(InputState::TEXT_BOX);
    }

    dialog->open(imagePath, text);
    menuManager->open(dialog);
}

void GameContext::openTextBox(TileSets t, int tile, const std::string& text)
{
    if (_gameState.top() == InputState::NORMAL)
    {
        setInputState(InputState::TEXT_BOX);
    }

    dialog->open(t, tile, text);
    menuManager->open(dialog);
}

void GameContext::openTextBox(const std::vector<const Speech*>* speech)
{
    if (_gameState.top() != InputState::MENU)
    {
        setInputState(InputState::TEXT_BOX);
    }

    dialog->open(speech);
    menuManager->open(dialog);
}

void GameContext::runScript(ScriptType script)
{
    switch (script)
    {
        case ScriptType::ITS_JUST_TRASH:
            scriptRunner
                .addStep(new ModifyEntitiesStep(*this, true))
                .addStep(new DialogueStep(this))
                .addStep(new ModifyEntitiesStep(*this, false));
            scriptRunner.run();
            setInputState(InputState::SCRIPT_RUNNING);
            break;
    }
}

void GameContext::toggleHitboxView()
{
    graphics->toggleHitboxView();
}

void GameContext::setInputState(InputState s)
{
    if (_gameState.top() != s)
    {
        _gameState.push(s);
    }
}

void GameContext::returnToPreviousGameState()
{
    _gameState.pop();
}

void GameContext::toggleFrameRate()
{
    _showFrameRate = !_showFrameRate;
}

void GameContext::openMenu(MenuType type)
{
    if (type == MenuType::PAUSE)
    {
        audio.playPauseMenuMusic(true);
    }
    menuManager->open(type);
    setInputState(InputState::MENU);
}

void GameContext::onAllMenusClosed()
{
    audio.playPauseMenuMusic(false);
    returnToPreviousGameState();
}

void GameContext::closeAllMenus()
{
    menuManager->closeAllMenus();
}

void GameContext::loadScene(Scenes scene)
{
    loadScene(scene, -1);
}

void GameContext::loadScene(Scenes scene, int spawnId)
{
    _sceneLoadRequested = true;
    _sceneToLoad = scene;
    _spawnId = spawnId;
}

// TODO: This is getting really mangled. Fix movement code and move it
void processPlayerMovement(GameContext* context, Entity& e, const float timeStep) {
    float startX = e.pos.x;
    float startY = e.pos.y;
    int xVelocity = e.vel.xVel;
    int yVelocity = e.vel.yVel;
    if (xVelocity < 0)
    {
        e.pos.x += -120 * timeStep;
    	e.vel.xVel += 2;
    }
    else if (xVelocity > 0)
    {
    	e.pos.x += 120 * timeStep;
    	e.vel.xVel += -2;
    }
    
    if (yVelocity < 0)
    {
    	e.pos.y += -120 * timeStep;
    	e.vel.yVel += 2;
    }
    else if (yVelocity > 0)
    {
    	e.pos.y += 120 * timeStep;
    	e.vel.yVel += -2;
    }
    
    if (startX < -30)
    {
        e.pos.x = SCREEN_WIDTH + 30;
        context->broadcast(EventType::CHANGE_SCENE, e);
    } else if (startX > SCREEN_WIDTH + 30)
    {
    	e.pos.x = -30;
        context->broadcast(EventType::CHANGE_SCENE, e);
    }
    
    if (startY < -30)
    {
    	e.pos.y = SCREEN_HEIGHT + 30;
        context->broadcast(EventType::CHANGE_SCENE, e);
    } else if (startY > SCREEN_HEIGHT + 30)
    {
    	e.pos.y = -30;
        context->broadcast(EventType::CHANGE_SCENE, e);
    }
    
    context->resolveCollision(e, startX, startY);
}

// TODO: This is cool, but needs to be rethought
float enemy_time = 0.0f;
float enemy_randomAmountOfTime = 0.0f;
int enemy_randomThing = 0;
void processEnemyMovement(GameContext* context, Entity& e, const float timeStep) {
    int startX = e.pos.x;
    int startY = e.pos.y;

    if (distanceBetween(e, *context->player) < 150.0f)
    {
        e.move(relativeDirection(e, *context->player), timeStep);
    }
    else
    {
        enemy_time += timeStep;
        if (enemy_time > enemy_randomAmountOfTime)
        {
            enemy_randomAmountOfTime = (float)(std::rand() % 4);
            enemy_randomThing = std::rand() % 8;
            enemy_time = 0.0f;
        }

        switch (context->graphics->getPosition(startX, startY))
        {
            case WindowPosition::RIGHT:
                enemy_randomThing = 2;
                break;
            case WindowPosition::LEFT:
                enemy_randomThing = 3;
                break;
            case WindowPosition::BELOW:
                enemy_randomThing = 0;
                break;
            case WindowPosition::ABOVE:
                enemy_randomThing = 1;
                break;
            default:
                break;
        }

        switch (enemy_randomThing)
        {
            case 0:
                e.move(Direction::UP, timeStep);
                break;
            case 1:
                e.move(Direction::DOWN, timeStep);
                break;
            case 2:
                e.move(Direction::LEFT, timeStep);
                break;
            case 3:
                e.move(Direction::RIGHT, timeStep);
                break;
            default:
                break;
        }
    }
    context->resolveCollision(e, startX, startY);
}

void GameContext::run()
{
    FrameRate frameRate(graphics);
    TimeStep timeStep;
    SDL_Event event;
    float lastTime = 0;
    audio.play("audio/back_pocket.wav");
    _level->load(Levels::LONELY_TOWN);
    _level->load(Scenes::LONELY_TOWN_OUTSKIRTS);
    while (true)
    {
        float currentTime = ((float)SDL_GetTicks()) / 1000;
        float localTimeStep = currentTime - lastTime;
        timeStep.setTimeStep(localTimeStep);
        lastTime = currentTime;
        // EVENT HANDLING
        // 
        // Reset "click" keys so multiple events don't get fired
        // This may cause a bug, because if there are multiple things
        // looking for a "click" event in the same loop, both may be
        // fired.
        input.upClick = false;
        input.downClick = false;
        input.leftClick = false;
        input.rightClick = false;
        input.select = false;
        input.back = false;
        input.pause = false;
        input.debug = 0;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                break;
            }
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            {
                SDL_KeyboardEvent& key = event.key;
                uint32_t eventType = event.type;
                bool isKeyDown = eventType == SDL_KEYDOWN;

                switch (key.keysym.sym) {
                    case SDLK_UP:
                        input.up = isKeyDown;
                        input.upClick = isKeyDown;
                        break;
                    case SDLK_DOWN:
                        input.down = isKeyDown;
                        input.downClick = isKeyDown;
                        break;
                    case SDLK_LEFT:
                        input.left = isKeyDown;
                        input.leftClick = isKeyDown;
                        break;
                    case SDLK_RIGHT:
                        input.right = isKeyDown;
                        input.rightClick = isKeyDown;
                        break;
                    case SDLK_f:
                        input.select = isKeyDown && !input.select;
                        break;
                    case SDLK_d:
                        input.back = isKeyDown && !input.back;
                        break;
                    case SDLK_RETURN:
                        input.pause = isKeyDown && !input.pause;
                        break;
                    case SDLK_r:
                        input.debug = isKeyDown && input.debug == 0 ? DEBUG_FRAME_RATE : 0;
                        break;
                    case SDLK_b:
                        input.debug = isKeyDown && input.debug == 0 ? DEBUG_TOGGLE_HIT_BOX : 0;
                        break;
                    default:
                        break;
                }
            }
        }
        // END

        if (_gameState.top() == InputState::SCRIPT_RUNNING)
        {
            if (scriptRunner.isRunning())
            {
                scriptRunner.processStep();
            }
            else
            {
                returnToPreviousGameState();
            }
            
        }

        // HANDLE GLOBAL INPUT
        if (input.debug & DEBUG_FRAME_RATE)
        {
            toggleFrameRate();
        }
        if (input.debug & DEBUG_TOGGLE_HIT_BOX)
        {
            toggleHitboxView();
        }
        // END

        // HANDLE INPUT
        switch (_gameState.top())
        {
            case InputState::MENU:
                pauseMenuStateHandler(*this);
                break;
            case InputState::SCRIPT_RUNNING:
                 //scriptStateHandler(*this);
                 break;
            case InputState::TEXT_BOX:
                textBoxStateHandler(*this);
                break;
            case InputState::NORMAL:
            default:
                normalStateHandler(*this);
                break;
        }
        // END

        // TODO: Should processing movement also be in the scene?
        if (_gameState.top() == InputState::NORMAL)
        {
            player->tick(localTimeStep);
            processPlayerMovement(this, *player, localTimeStep);
            for (short i = 1; i < entities.back; i++) {
                if (entities.entities[i].type == EntityType::ENEMY) {
                    processEnemyMovement(this, entities.entities[i], localTimeStep);
                }
            }
            // This spawns an enemy
            _level->update(localTimeStep);
        }

        _level->draw(timeStep);
        menuManager->draw(timeStep);
        if (_showFrameRate)
        {
            frameRate.draw(localTimeStep);
        }
        graphics->present();
        SDL_Delay(1000 / GraphicsContext::FRAME_RATE);
        if (_sceneLoadRequested)
        {
            _sceneLoadRequested = false;
            _level->load(_sceneToLoad, _spawnId);
            _spawnId = -1;
        }
    }
}
