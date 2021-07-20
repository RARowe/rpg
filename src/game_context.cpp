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
#include "editor.c"
#include "scene_file_reader.h"
#include "input.c"

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

void GameContext::requestOpenTextBox(const char* image, const char* text) {
    _openTextBoxRequested = true;
    textBox.imagePath = image;
    textBox.text = text;
    textBox.useTileset = false;
}

void GameContext::requestOpenTextBox(unsigned int t, int tile, const char* text) {
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
        // TODO: This way of drawing textures is no longer supported
        //graphics->drawTexture(0, y, 160, 160, t->imagePath);
    }
    graphics->drawWrappedText(192, y, 32, 384, t->text);
}

void overworld_handle_input(const Input* in, PlayerInput* i) {
    memset(i, 0, sizeof(PlayerInput));
    i->up = input_is_down(in, SDLK_UP);
    i->upClick = input_is_pressed(in, SDLK_UP);
    i->down = input_is_down(in, SDLK_DOWN);
    i->downClick = input_is_pressed(in, SDLK_DOWN);
    i->left = input_is_down(in, SDLK_LEFT);
    i->leftClick = input_is_pressed(in, SDLK_LEFT);
    i->right = input_is_down(in, SDLK_RIGHT);
    i->rightClick = input_is_pressed(in, SDLK_RIGHT);
    i->select = input_is_pressed(in, SDLK_f) && !i->select;
    i->back = input_is_pressed(in, SDLK_d) && !i->back;
    i->pause = input_is_pressed(in, SDLK_RETURN) && !i->pause;
    i->r = input_is_pressed(in, SDLK_r);
    i->b = input_is_pressed(in, SDLK_b);
    i->e = input_is_pressed(in, SDLK_e);
}

void GameContext::run()
{
    Input i;
    float lastTime = 0;
    audio.play("resources/audio/back_pocket.wav");

    // TODO: REMOVE THIS
    SceneData scene = readSceneFile("resources/levels/lonely_town/", "outskirts.tmx");
    // END
    while (input_process(&i))
    {
        float currentTime = ((float)SDL_GetTicks()) / 1000;
        float localTimeStep = currentTime - lastTime;
        lastTime = currentTime;

        // HANDLE GLOBAL INPUT
        overworld_handle_input(&i, &input);
        if (input.r)
        {
            _showFrameRate = !_showFrameRate;
        }
        if (input.b)
        {
            toggleHitboxView();
        }

        if (input.e) {
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
                editor_handle_input(&i, &scene);
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
        graphics->drawTiles(scene.tileSet, scene.background, scene.backgroundSize);
        graphics->drawTiles(scene.tileSet, scene.midground, scene.midgroundSize);
        for (auto&& p : scene.tileSprites) {
            auto&& body = scene.bodies[p.first];
            graphics->drawTile(scene.tileSet, p.second, body.x, body.y, body.w, body.h);
        }
        for (auto&& w : scene.warpPoints) {
            auto&& body = scene.bodies[w.first];
            graphics->drawTile(0, (int)SpriteSheetTexture::WOODEN_DOOR_ROUNDED_WINDOW_CLOSED, body.x, body.y, body.w, body.h);
        }
        for (auto&& p : scene.solidEntities) {
            auto&& body = scene.bodies[p];
            graphics->drawBox(body.x, body.y, body.w, body.h, Color::WHITE, 100);
        }
        // Terrible player rendering
        Body& b = scene.bodies[0];
        graphics->drawBox(b.x, b.y, b.w, b.h, Color::BLUE, 255);
        graphics->drawTiles(scene.tileSet, scene.foreground, scene.foregroundSize);
        // End

        if (_gameState.top() == GameState::MENU) {
            menuManager->draw(localTimeStep);
        }
        if (_showFrameRate) {
			char b[4];
			sprintf(b, "%d", (int) (1.0f / localTimeStep));
			graphics->drawText(0, 0, 60, 30, b);
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
