#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include "game_context.h"
#include "game_math.h"
#include "frame_rate.h"
#include "editor.c"
#include "scene_file_reader.h"
#include "nfd.h"

GameContext::GameContext()
{
    // Init inventory
    // TODO: Magic inventory number
    inventory = (Inventory*)malloc(sizeof(Inventory) + sizeof(ItemType) * 112);
    memset(&input, 0, sizeof(PlayerInput));
    graphics = new GraphicsContext("test", SCREEN_WIDTH, SCREEN_HEIGHT, "resources/");
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

// TODO: This is getting really mangled. Fix movement code and move it
void processPlayerMovement(GameContext* context, Body& body, Velocity& vel, const float timeStep) {
    float startX = body.x;
    float startY = body.y;
    int xVelocity = vel.xVel;
    int yVelocity = vel.yVel;
    if (xVelocity < 0)
    {
        body.x += -120 * timeStep;
    	vel.xVel += 2;
    }
    else if (xVelocity > 0)
    {
    	body.x += 120 * timeStep;
    	vel.xVel += -2;
    }
    
    if (yVelocity < 0)
    {
    	body.y += -120 * timeStep;
    	vel.yVel += 2;
    }
    else if (yVelocity > 0)
    {
    	body.y += 120 * timeStep;
    	vel.yVel += -2;
    }
    
    if (startX < -30)
    {
        body.x = SCREEN_WIDTH + 30;
    } else if (startX > SCREEN_WIDTH + 30)
    {
    	body.x = -30;
    }
    
    if (startY < -30)
    {
    	body.y = SCREEN_HEIGHT + 30;
    } else if (startY > SCREEN_HEIGHT + 30)
    {
    	body.y = -30;
    }
}

void draw_textbox(GraphicsContext* graphics, const TextBox* t, const Body* body, const float timeStep)
{
    const int playerY = body->y;
    const int y = playerY > 256 ? 0 : 256;

    graphics->drawBox(0, y, 608, 160, Color::BLUE, 255);
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

void open_dialog() {
	nfdchar_t *outPath = NULL;
	nfdresult_t result = NFD_OpenDialog( "png,jpg;pdf", ".", &outPath );

	if ( result == NFD_OKAY ) {
		puts("Success!");
		puts(outPath);
		free(outPath);
	}
	else if ( result == NFD_CANCEL ) {
		puts("User pressed cancel.");
	}
	else {
		printf("Error: %s\n", NFD_GetError() );
	}
}

void GameContext::run()
{
    FrameRate frameRate(graphics);
    SDL_Event event;
    float lastTime = 0;
    audio.play("audio/back_pocket.wav");

    // TODO: REMOVE THIS
    SceneData scene = readSceneFile("resources/game_data/levels/lonely_town/", "outskirts.tmx");
    // END
    bool drawBackground = true;
    bool drawMidground = true;
    bool drawForeground = true;
    while (true)
    {
        float currentTime = ((float)SDL_GetTicks()) / 1000;
        float localTimeStep = currentTime - lastTime;
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
                    case SDLK_e:
                        input.debug = isKeyDown && input.debug == 0 ? DEBUG_EDITOR : 0;
                        break;
                    default:
                        break;
                }
            }
        } else {
            memset(&event, 0, sizeof(SDL_Event)); 
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

        if (input.debug & DEBUG_EDITOR) {
            if (_gameState.top() == GameState::EDITOR) {
                _gameState.pop();
            } else {
                _gameState.push(GameState::EDITOR);
                audio.stop();
            }

        }
        // END

        // HANDLE INPUT
        switch (_gameState.top())
        {
            case GameState::EDITOR:
                editor_handle_input(&event, &drawBackground, &drawMidground, &drawForeground, &scene);
                break;
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
                const int MAX_VELOCITY = 4;
                if (input.left)
                {
                    scene.vel.xVel = -MAX_VELOCITY;
                }
                if (input.right)
                {
                    scene.vel.xVel = MAX_VELOCITY;
                }
                if (input.up)
                {
                    scene.vel.yVel = -MAX_VELOCITY;
                }
                if (input.down)
                {
                    scene.vel.yVel = MAX_VELOCITY;
                }
                if (input.pause)
                {
                    openMenu(MenuType::PAUSE);
                }
                
                //scene_process_interaction(this, sceneData, &input);
                break;
        }
        // END

        // TODO: Should processing movement also be in the scene?
        //       This should go away for sure
        if (_gameState.top() == GameState::NORMAL)
        {
            float startX = scene.bodies[0].x;
            float startY = scene.bodies[0].y;
            processPlayerMovement(this, scene.bodies[0], scene.vel, localTimeStep);

            int i = 0;
            for (auto&& p : scene.solidEntities) {
                if (i++ == 0) { continue; }
                auto&& body = scene.bodies[p];
                if (entitiesCollide(scene.bodies[0], body)) {
                    scene.bodies[0].x = startX;
                    scene.bodies[0].y = startY;
                    break;
                }
            }
        }

        graphics->drawBox(0, 0, 1000, 1000, Color::BLACK, 255);
        // Draw level
        if (drawBackground) { graphics->drawTiles(scene.tileSet, scene.background); }
        if (drawMidground) { graphics->drawTiles(scene.tileSet, scene.midground); }
        for (auto&& p : scene.tileSprites) {
            auto&& body = scene.bodies[p.first];
            graphics->drawTile(scene.tileSet, p.second, body.x, body.y, body.w, body.h);
        }
        for (auto&& w : scene.warpPoints) {
            auto&& body = scene.bodies[w.first];
            graphics->drawTile(TileSets::OUTDOOR, (int)SpriteSheetTexture::WOODEN_DOOR_ROUNDED_WINDOW_CLOSED, body.x, body.y, body.w, body.h);
        }
        for (auto&& p : scene.solidEntities) {
            auto&& body = scene.bodies[p];
            graphics->drawBox(body.x, body.y, body.w, body.h, Color::RED, 100);
        }
        // Terrible player rendering
        Body& b = scene.bodies[0];
        graphics->drawBox(b.x, b.y, b.w, b.h, Color::BLUE, 255);
        if (drawForeground) { graphics->drawTiles(scene.tileSet, scene.foreground); }
        // End

        if (_gameState.top() == GameState::MENU) {
            menuManager->draw(localTimeStep);
        }
        if (_showFrameRate)
        {
            frameRate.draw(localTimeStep);
        }

        if (_gameState.top() == GameState::TEXTBOX) {
            // TODO
            //draw_textbox(graphics, &textBox, player, localTimeStep);
        }

        if (_gameState.top() == GameState::EDITOR) {
            editor_draw(graphics);
        }

        graphics->present();
        SDL_Delay(1000 / GraphicsContext::FRAME_RATE);

        if (_openTextBoxRequested) {
            _openTextBoxRequested = false;
            _gameState.push(GameState::TEXTBOX);
        }
    }
}
