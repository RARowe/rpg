#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include "game_context.h"
#include "game_math.h"
#include "frame_rate.h"
#include "scenes.h"
#include "time_step.h"
#include "levels.h"

// TODO: This could go in better place
static void processPlayerInput(GameContext& c, Entity& e, PlayerInput& i)
{
    const int MAX_VELOCITY = 4;
    if (i.left)
    {
        e.vel.xVel = -MAX_VELOCITY;
        e.direction = Direction::LEFT;
    }
    if (i.right)
    {
        e.vel.xVel = MAX_VELOCITY;
        e.direction = Direction::RIGHT;
    }
    if (i.up)
    {
        e.vel.yVel = -MAX_VELOCITY;
        e.direction = Direction::UP;
    }
    if (i.down)
    {
        e.vel.yVel = MAX_VELOCITY;
        e.direction = Direction::DOWN;
    }
    if (i.select)
    {
    	c.broadcast(EventType::INTERACT, e);
    }
}

static void normalStateHandler(GameContext& c)
{
    if (c.input.pause)
    {
        c.openMenu(MenuType::PAUSE);
    }
    else
    {
        processPlayerInput(c, *c.player, c.input);
    }
}

GameContext::GameContext()
{
    // Init entities
    // TODO: use single allocation
    entities.size = 500;
    entities.back = 0;
    entities.entities = (Entity*)calloc(500, sizeof(Entity) * 500);
    // Init inventory
    // TODO: Magic inventory number
    inventory = (Inventory*)malloc(sizeof(Inventory) + sizeof(ItemType) * 112);
    memset(&input, 0, sizeof(PlayerInput));
    graphics = new GraphicsContext("test", SCREEN_WIDTH, SCREEN_HEIGHT, "resources/");
    _entityFactory = EntityFactory::getInstance(this);
    addEntity(EntityType::PLAYER);
    player = &entities.entities[0];
    _level = new Level(this);
    menuManager = MenuManager::getInstance(this);
    _gameState.push(GameState::NORMAL);
}
void dialogue_compile(const char* dialogue, Dialogue* d) {
    unsigned int currentStep = 0;
    unsigned int i = 0;
    DialogueStep* step;
    while (*dialogue != '\0') {
        step = &d->steps[currentStep++];

        i = 0;
        while (*dialogue != ':') {
            step->image[i++] = *dialogue++;
        }
        dialogue++;
        step->image[i] = '\0';

        i = 0;
        while (*dialogue != '\n' && *dialogue != '\0') {
            step->text[i++] = *dialogue++;
        }
        step->text[i] = '\0';

        if (*dialogue == '\n') {
            dialogue++;
        }
    }

    d->numberOfSteps = currentStep;
}

void read_file(const char* filepath, char* buffer) {
    FILE* f = fopen(filepath, "r");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    fread(buffer, sizeof(char), fsize, f);

    fclose(f);
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

void GameContext::addEnemy()
{
    Entity* e = &entities.entities[entities.back];
    entities.back++;
    _entityFactory->initEnemy(e);
}

// TODO: this could be in a better place
bool entitiesCollide(const Entity& e1, const Entity& e2) {
	if (e1.id == e2.id) { return false; }
    int x2 = e1.body.x+ e1.body.w,
        y2 = e1.body.y + e1.body.h,
        e2x2 = e2.body.x + e2.body.w,
        e2y2 = e2.body.y + e2.body.h;
    bool below = e2.body.y >= y2,
         above = e2y2 <= e1.body.y,
         left = e2x2 <= e1.body.x,
         right = e2.body.x >= x2;
    return !(below || above || left || right);
}
// TODO(COLLISION): This does not take into account if entities are the same
bool entitiesCollide(const Body& b1, const Body& b2) {
    int x2 = b1.x+ b1.w,
        y2 = b1.y + b1.h,
        b2x2 = b2.x + b2.w,
        b2y2 = b2.y + b2.h;
    bool below = b2.y >= y2,
         above = b2y2 <= b1.y,
         left = b2x2 <= b1.x,
         right = b2.x >= x2;
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
            int currentX = e.body.x;
            e.body.x = oldX;
            if (entitiesCollide(e, e2))
            {
                e.body.x = currentX;
                e.body.y = oldY;
                if (entitiesCollide(e, e2))
                {
                    e.body.x = oldX;
                }
            }
        }
    }

    for (auto&& eid : sceneData->solidEntities) {
        auto&& body = sceneData->gameEntities[eid];
        if (entitiesCollide(e.body, body))
        {
            int currentX = e.body.x;
            e.body.x = oldX;
            if (entitiesCollide(e.body, body))
            {
                e.body.x = currentX;
                e.body.y = oldY;
                if (entitiesCollide(e.body, body))
                {
                    e.body.x = oldX;
                }
            }
            // TODO: More sane warp point code
            if (sceneData->warpPoints.count(eid)) {
                //TODO(ENTITIES): This needs to be readded after player has moved over to new entity system
                //if (eid != player->id) { return; }
                WarpPoint& warp = sceneData->warpPoints[eid];
                loadScene(warp.sceneToLoad, warp.destinationSpawn);
                audio.playSound("audio/door.ogg");
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

void GameContext::toggleHitboxView()
{
    graphics->toggleHitboxView();
}

void GameContext::setGameState(GameState s)
{
    if (_gameState.top() != s)
    {
        _gameState.push(s);
    }
}

void GameContext::toggleFrameRate()
{
    _showFrameRate = !_showFrameRate;
}

void GameContext::requestOpenTextBox(const char* image, const char* text) {
    _openTextBoxRequested = true;
    textBox.imagePath = image;
    textBox.text = text;
    textBox.useTileset = false;
}

void GameContext::requestOpenTextBox(TileSets t, int tile, const char* text) {
    _openTextBoxRequested = true;
    textBox.tileSet = t;
    textBox.tile = tile;
    textBox.text = text;
    textBox.useTileset = true;
}

void GameContext::requestOpenDialogue(const Dialogue* d) {
    _openDialogueRequested = true;
}

void GameContext::openMenu(MenuType type)
{
    if (type == MenuType::PAUSE)
    {
        audio.playPauseMenuMusic(true);
    }
    menuManager->open(type);
    setGameState(GameState::MENU);
}

void GameContext::onAllMenusClosed()
{
    audio.playPauseMenuMusic(false);
    _gameState.pop();
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
    float startX = e.body.x;
    float startY = e.body.y;
    int xVelocity = e.vel.xVel;
    int yVelocity = e.vel.yVel;
    if (xVelocity < 0)
    {
        e.body.x += -120 * timeStep;
    	e.vel.xVel += 2;
    }
    else if (xVelocity > 0)
    {
    	e.body.x += 120 * timeStep;
    	e.vel.xVel += -2;
    }
    
    if (yVelocity < 0)
    {
    	e.body.y += -120 * timeStep;
    	e.vel.yVel += 2;
    }
    else if (yVelocity > 0)
    {
    	e.body.y += 120 * timeStep;
    	e.vel.yVel += -2;
    }
    
    if (startX < -30)
    {
        e.body.x = SCREEN_WIDTH + 30;
        context->broadcast(EventType::CHANGE_SCENE, e);
    } else if (startX > SCREEN_WIDTH + 30)
    {
    	e.body.x = -30;
        context->broadcast(EventType::CHANGE_SCENE, e);
    }
    
    if (startY < -30)
    {
    	e.body.y = SCREEN_HEIGHT + 30;
        context->broadcast(EventType::CHANGE_SCENE, e);
    } else if (startY > SCREEN_HEIGHT + 30)
    {
    	e.body.y = -30;
        context->broadcast(EventType::CHANGE_SCENE, e);
    }
    
    context->resolveCollision(e, startX, startY);
}

// TODO: This is cool, but needs to be rethought
float enemy_time = 0.0f;
float enemy_randomAmountOfTime = 0.0f;
int enemy_randomThing = 0;
void processEnemyMovement(GameContext* context, Entity& e, const float timeStep) {
    int startX = e.body.x;
    int startY = e.body.y;

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

void GameContext::registerStateTransition(Entity* e, int state, float time) {
    e->state = state;
    stateTransitions[e->id] = time;
}

Entity* GameContext::getEntityById(const int eid) {
    for (int i = 0; i < entities.size; i++) {
        if (entities.entities[i].id == eid) {
            return &entities.entities[i];
        }
    }
    // This should never not return a value if everything is working correctly
    // Adding junk value to supress warning
    puts("FATAL: getEntityById returning null pointer. This should never happen.");
    return nullptr;
}

// TODO: STATE remove this
static void processStateTransitions(GameContext* c, const float timeStep) {
    for (auto&& p : c->stateTransitions) {
        p.second -= timeStep;
        if (p.second < 0) {
            c->getEntityById(p.first)->state = 0;
        }
    }
}

// TODO(INTERACT): This could be in better place
static inline bool point_in_body(const Body& b, const Point& p)
{
	return p.x >= b.x &&
        p.x <= b.x + b.w &&
        p.y >= b.y &&
        p.y <= b.y + b.h;
}

static inline void calculate_cursor(Point& c, const Entity* e)
{
    switch (e->direction)
    {
        case Direction::LEFT:
            c.x = e->body.x - 10;
            c.y = e->body.y + (e->body.h / 2);
            break;
        case Direction::RIGHT:
            c.x = e->body.x + e->body.w + 10;
            c.y = e->body.y + (e->body.h / 2);
            break;
        case Direction::UP:
            c.x = e->body.x + (e->body.w / 2);
            c.y = e->body.y - 10;
            break;
        case Direction::DOWN:
            c.x = e->body.x + (e->body.w / 2);
            c.y = e->body.y + e->body.h + 10;
            break;
    }
}

void GameContext::scene_process_interaction(GameContext* c, SceneData* s, const PlayerInput* i) {
    if (!i->select) { return; }
    Point p;
    for (auto&& pair : s->textInteractions) {
        Body& b = s->gameEntities[pair.first];
        calculate_cursor(p, c->player);
        if (point_in_body(b, p)) {
            c->requestOpenTextBox("tim.png", pair.second.c_str());
            s->interactionCounter++;
        }
    }
}

void draw_textbox(GraphicsContext* graphics, const TextBox* t, const Entity* player, const float timeStep)
{
    int playerY = player->body.y;
    int y = playerY > 256 ? 0 : 256;
    graphics->drawBox(0, y, 608, 160, Color::BLUE);
    if (t->useTileset)
    {
        graphics->drawTile(t->tileSet, t->tile, 0, y, 160, 160);
    }
    else
    {
        graphics->drawTexture(0, y, 160, 160, t->imagePath);
    }
    graphics->drawWrappedText(192, y, 32, 384, t->text);
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
            case GameState::TEXTBOX:
                if (input.select) {
                    _gameState.pop();
                }
                break;
            case GameState::MENU:
                menuManager->processInput(&input);
                break;
            case GameState::NORMAL:
            default:
                normalStateHandler(*this);
                scene_process_interaction(this, sceneData, &input);
                break;
        }
        // END

        // TODO: Should processing movement also be in the scene?
        if (_gameState.top() == GameState::NORMAL)
        {
            processStateTransitions(this, localTimeStep);
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
        if (_gameState.top() == GameState::MENU) {
            menuManager->draw(timeStep);
        }
        if (_showFrameRate)
        {
            frameRate.draw(localTimeStep);
        }

        if (_gameState.top() == GameState::TEXTBOX) {
            draw_textbox(graphics, &textBox, player, localTimeStep);
        }

        graphics->present();
        SDL_Delay(1000 / GraphicsContext::FRAME_RATE);
        if (_sceneLoadRequested)
        {
            _sceneLoadRequested = false;
            _level->load(_sceneToLoad, _spawnId);
            _spawnId = -1;
        }

        if (_openTextBoxRequested) {
            _openTextBoxRequested = false;
            _gameState.push(GameState::TEXTBOX);
        }
    }
}
