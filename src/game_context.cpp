#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include "game_context.h"
#include "editor.c"
#include "input.c"
#include "modal.c"

GameContext::GameContext()
{
    memset(&input, 0, sizeof(PlayerInput));
    graphics = new GraphicsContext("test", SCREEN_WIDTH, SCREEN_HEIGHT, "resources/");
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

void GameContext::requestOpenModal(char** options, int numberOfOptions, int* result) {
    // TODO: This should reject requests with too many options
    _openModalRequested = true;
    
    for (int i = 0; i < numberOfOptions; i++) {
        modal.options[i] = options[i];
    }

    modal.numberOfOptions = numberOfOptions;
    modal.result = result;
}

void GameContext::requestOpenTilePicker(unsigned int id, int* tile) {
    // TODO: This could blow up
    _openTilePickerRequested = true;
    tile_picker_init(&tilePicker, id, &(graphics->textureCache[id]));
    _tile = tile;
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
    i->esc = input_is_pressed(in, SDLK_ESCAPE);
}

void calculate_cursor(Point* p, Body* b) {
    p->x = b->x;
    p->y = b->y - 5;
}

void scene_process_interaction(GameContext* c, SceneData* s, const PlayerInput* i) {
    if (!i->select) { return; }
    Point p;
    Body* player = &(s->bodies[0]);
    for (auto&& pair : s->textInteractions) {
        Body& b = s->bodies[pair.first];
        calculate_cursor(&p, player);
        if (point_in_body(b, p)) {
            c->requestOpenTextBox("tim.png", pair.second.c_str());
        }
    }
}

void GameContext::run()
{
    Input i;
    float lastTime = 0;

    // TODO: REMOVE THIS
    SceneData scene;
    for (int j = 0; j < scene.backgroundSize; j++) {
        scene.background[j] = -1;
        scene.midground[j] = -1;
        scene.foreground[j] = -1;
    }
    Body b = { 100, 100, 32, 32 };
    scene.bodies[0] = b;
    scene.vel.xVel = 0;
    scene.vel.yVel = 0;
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

        if (input.e && _gameState.top() == GameState::NORMAL) {
            _gameState.push(GameState::EDITOR);
        }

        if (input.esc && _gameState.top() == GameState::EDITOR) {
            _gameState.pop();
        }
        // END

        // HANDLE INPUT
        switch (_gameState.top())
        {
            case GameState::EDITOR:
                editor_handle_input(this, graphics, &i, &scene);
                break;
            case GameState::TEXTBOX:
                if (input.select) {
                    _gameState.pop();
                }
                break;
            case GameState::MODAL:
                if (modal_handle_input(&i, &modal)) {
                    // TODO: This is bad, because we should request a state change,
                    //       not automatically change it ourselves
                    _gameState.pop();
                }
                break;
            case GameState::TILE_PICKER:
                if (tile_picker_handle_input(&i, &tilePicker)) {
                    // TODO: This is bad, because we should request a state change,
                    //       not automatically change it ourselves
                    *_tile = tilePicker.tile;
                    _gameState.pop();
                }
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
                
                scene_process_interaction(this, &scene, &input);
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
        for (auto&& p : scene.solidEntities) {
            auto&& body = scene.bodies[p];
            graphics->drawBox(body.x, body.y, body.w, body.h, Color::WHITE, 100);
        }
        // Terrible player rendering
        Body& b = scene.bodies[0];
        graphics->drawBox(b.x, b.y, b.w, b.h, Color::BLUE, 255);
        graphics->drawTiles(scene.tileSet, scene.foreground, scene.foregroundSize);
        // End

        if (_showFrameRate) {
			char b[4];
			sprintf(b, "%d", (int) (1.0f / localTimeStep));
			graphics->drawText(0, 0, 60, 30, b);
        }

        if (_gameState.top() == GameState::TEXTBOX) {
            draw_textbox(graphics, &textBox, &(scene.bodies[0]), localTimeStep);
        }

        if (_gameState.top() == GameState::EDITOR) {
            editor_draw(graphics,localTimeStep);
        }

        if (_gameState.top() == GameState::MODAL) {
            modal_draw(graphics, &modal, localTimeStep);
        }

        if (_gameState.top() == GameState::TILE_PICKER) {
            graphics->drawTilesetPicker(&tilePicker);
        }

        graphics->present();
        SDL_Delay(1000 / GraphicsContext::FRAME_RATE);

        if (_openTextBoxRequested) {
            _openTextBoxRequested = false;
            _gameState.push(GameState::TEXTBOX);
        } else if (_openModalRequested) {
            _openModalRequested = false;
            _gameState.push(GameState::MODAL);
        } else if (_openTilePickerRequested) {
            _openTilePickerRequested = false;
            _gameState.push(GameState::TILE_PICKER);
        }
    }
}
