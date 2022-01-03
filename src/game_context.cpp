#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "game_context.h"
#include "editor.c"
#include "input.c"
#include "modal.c"

bool entitiesCollide(const Body* b1, const Body* b2) {
    int x2 = b1->x+ b1->w,
        y2 = b1->y + b1->h,
        b2x2 = b2->x + b2->w,
        b2y2 = b2->y + b2->h;
    bool below = b2->y >= y2,
         above = b2y2 <= b1->y,
         left = b2x2 <= b1->x,
         right = b2->x >= x2;
    return !(below || above || left || right);
}

void GameContext::requestOpenTextBox(const char* image, const char* text) {
    _openTextBoxRequested = true;
    textBox.imagePath = image;
    textBox.text = text;
    textBox.useTileset = false;
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

void GameContext::requestOpenTilePicker(int* tile) {
    _openTilePickerRequested = true;
    _tile = tile;
}

void GameContext::requestSceneSave() {
    _sceneSaveRequested = true;
}

void GameContext::requestSceneLoad() {
    _sceneLoadRequested = true;
}

// TODO: This is getting really mangled. Fix movement code and move it
void processPlayerMovement(GameContext* context, Body* body, Velocity& vel, const float timeStep) {
    float startX = body->x;
    float startY = body->y;
    int xVelocity = vel.xVel;
    int yVelocity = vel.yVel;
    if (xVelocity < 0) {
        body->x += -120 * timeStep;
    	vel.xVel += 2;
    } else if (xVelocity > 0) {
    	body->x += 120 * timeStep;
    	vel.xVel += -2;
    }
    
    if (yVelocity < 0) {
    	body->y += -120 * timeStep;
    	vel.yVel += 2;
    } else if (yVelocity > 0) {
    	body->y += 120 * timeStep;
    	vel.yVel += -2;
    }
    
    if (startX < -30) {
        body->x = SCREEN_WIDTH + 30;
    } else if (startX > SCREEN_WIDTH + 30) {
    	body->x = -30;
    }
    
    if (startY < -30) {
    	body->y = SCREEN_HEIGHT + 30;
    } else if (startY > SCREEN_HEIGHT + 30) {
    	body->y = -30;
    }
}

void draw_textbox(Graphics* graphics, const TextBox* t, const Body* body, const float timeStep) {
    const int playerY = body->y;
    const int y = playerY > 256 ? 0 : 256;

    graphics->drawBox(0, y, 608, 160, Color::BLUE, 255);
    if (t->useTileset) {
        graphics->drawTile(t->tileSet, t->tile, 0, y, 160, 160);
    } else {
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
    Body* player = entities_get_body(s, 0);
    for (auto&& pair : s->textInteractions) {
        Body* b = entities_get_body(s, pair.first);
        calculate_cursor(&p, player);
        if (point_in_body(b, &p)) {
            c->requestOpenTextBox("tim.png", pair.second.c_str());
        }
    }
}

void scene_save(const SceneData* s) {
    char path[256];
    strcat(path, "resources/");
    strcat(path, s->name);

    FILE* f = fopen(path, "w");

    /* Write next entity_id */
    fprintf(f, "next_id=%d\n", s->nextEntityId);

    // Write tiles
    for (int i = 0; i < 247; i++) {
        fprintf(f, "%d,", s->background[i]);
    }
    fputs("\n", f);
    for (int i = 0; i < 247; i++) {
        fprintf(f, "%d,", s->midground[i]);
    }
    fputs("\n", f);
    for (int i = 0; i < 247; i++) {
        fprintf(f, "%d,", s->foreground[i]);
    }
    fputs("\n", f);

    // Write Bodies
    fprintf(f, "%lu ", s->bodies.size() - 1);
    for (auto&& pair : s->bodies) {
        int id = pair.first;
        Body b = pair.second;
        if (id == 0) { continue; }
        fprintf(f, "%d:%f,%f,%hd,%hd$", id, b.x, b.y, b.w, b.h);
    }
    fputs("\n", f);

    /* Write Solid entities */
    fprintf(f, "%lu ", s->solidEntities.size());
    for (int id : s->solidEntities) {
        fprintf(f, "%d,", id);
    }
    fputs("\n", f);
    
    /* Write text interactions */
    fprintf(f, "%lu ", s->textInteractions.size());
    for (auto&& pair : s->textInteractions) {
        int id = pair.first;
        std::string s = pair.second;
        if (id == 0) { continue; }
        fprintf(f, "%d:%s$$", id, s.c_str());
    }
    fputs("\n", f);

    fclose(f);
}

void scene_load(SceneData* s) {
    FILE* f = fopen("resources/test.level", "r");

    fscanf(f, "next_id=%d\n", &(s->nextEntityId));

    for (int i = 0; i < 247; i++) {
        fscanf(f, "%d,", &(s->background[i]));
    }
    // Skip newline
    fseek(f, 1L, SEEK_CUR);
    for (int i = 0; i < 247; i++) {
        fscanf(f, "%d,", &(s->midground[i]));
    }
    fseek(f, 1L, SEEK_CUR);
    for (int i = 0; i < 247; i++) {
        fscanf(f, "%d,", &(s->foreground[i]));
    }
    fseek(f, 1L, SEEK_CUR);

    // Read bodies
    int numberOfBodies = 0;
    fscanf(f, "%d ", &numberOfBodies);
    int id;
    Body b;
    for (int i = 0; i < numberOfBodies; i++) {
        fscanf(f, "%d:%f,%f,%hd,%hd$", &id, &b.x, &b.y, &b.w, &b.h);
        s->bodies[id] = b;
    }
    fseek(f, 1L, SEEK_CUR);

    /* Read Solid entities */
    int numberOfSolidEntities = 0;
    fscanf(f, "%d ", &numberOfSolidEntities);
    for (int i = 0; i < numberOfSolidEntities; i++) {
        fscanf(f, "%d,", &id);
        s->solidEntities.insert(id);
    }
    fseek(f, 1L, SEEK_CUR);

    /* Read text interactions */
    int numberOfTextInteractions = 0;
    char textBuffer[1024];
    fscanf(f, "%d ", &numberOfTextInteractions);
    for (int i = 0; i < numberOfTextInteractions; i++) {
        fscanf(f, "%d:%[^$\n]$$", &id, textBuffer);
        s->textInteractions[id] = textBuffer;
    }
    fseek(f, 1L, SEEK_CUR);

    fclose(f);
}

void tile_picker_init(TilePicker* p, unsigned int id, Texture* t) {
    p->tilesetMeta.id = id;
    p->tilesetMeta.hTiles = ((t->w - 16) / 17) + 1;
    p->tilesetMeta.vTiles = ((t->h - 16) / 17) + 1;
    p->tilesetMeta.totalTiles = p->tilesetMeta.hTiles * p->tilesetMeta.vTiles;
    p->tile = 0;
}

void GameContext::run() {
    memset(&input, 0, sizeof(PlayerInput));
    graphics.init("RPG", SCREEN_WIDTH, SCREEN_HEIGHT, "resources/");
    _gameState.push(GameState::NORMAL);
    tile_picker_init(&tilePicker, 0, &(graphics.textureCache[0]));
    Input i;
    float lastTime = 0;

    // TODO: REMOVE THIS
    for (int j = 0; j < scene.backgroundSize; j++) {
        scene.background[j] = -1;
        scene.midground[j] = -1;
        scene.foreground[j] = -1;
    }
    // TODO: Fix player to have constant identifier
    Body b = { 100, 100, 32, 32 };
    scene.bodies[0] = b;
    scene.vel.xVel = 0;
    scene.vel.yVel = 0;
    // END
    while (input_process(&i)) {
        float currentTime = ((float)SDL_GetTicks()) / 1000;
        float localTimeStep = currentTime - lastTime;
        lastTime = currentTime;

        // HANDLE GLOBAL INPUT
        overworld_handle_input(&i, &input);
        if (input.r) {
            _showFrameRate = !_showFrameRate;
        }

        if (input.e && _gameState.top() == GameState::NORMAL) {
            _gameState.push(GameState::EDITOR);
        }

        if (input.esc && _gameState.top() == GameState::EDITOR) {
            _gameState.pop();
        }
        // END

        // HANDLE INPUT
        switch (_gameState.top()) {
            case GameState::EDITOR:
                editor_handle_input(this, &graphics, &i, &scene);
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
                if (input.left) {
                    scene.vel.xVel = -MAX_VELOCITY;
                }
                if (input.right) {
                    scene.vel.xVel = MAX_VELOCITY;
                }
                if (input.up) {
                    scene.vel.yVel = -MAX_VELOCITY;
                }
                if (input.down) {
                    scene.vel.yVel = MAX_VELOCITY;
                }
                
                scene_process_interaction(this, &scene, &input);
                break;
        }
        // END

        if (_gameState.top() == GameState::NORMAL) {
            Body* player = entities_get_body(&scene, 0);
            float startX = player->x;
            float startY = player->y;
            processPlayerMovement(this, player, scene.vel, localTimeStep);

            for (auto&& p : scene.solidEntities) {
                Body* body = entities_get_body(&scene, p);
                if (entitiesCollide(player, body)) {
                    player->x = startX;
                    player->y = startY;
                    break;
                }
            }
        }

        graphics.drawBox(0, 0, 1000, 1000, Color::BLACK, 255);
        // Draw level
        graphics.drawTiles(scene.tileSet, scene.background, scene.backgroundSize);
        graphics.drawTiles(scene.tileSet, scene.midground, scene.midgroundSize);
        for (auto&& p : scene.tileSprites) {
            Body* body = entities_get_body(&scene, p.first);
            graphics.drawTile(scene.tileSet, p.second, body->x, body->y, body->w, body->h);
        }

        if (_gameState.top() == GameState::EDITOR) {
            for (auto&& p : scene.solidEntities) {
                Body* body = entities_get_body(&scene, p);
                graphics.drawBox(body->x, body->y, body->w, body->h, Color::WHITE, 100);
            }
        }
        // Terrible player rendering
        Body* b = entities_get_body(&scene, 0);
        graphics.drawBox(b->x, b->y, b->w, b->h, Color::BLUE, 255);
        graphics.drawTiles(scene.tileSet, scene.foreground, scene.foregroundSize);
        // End

        if (_showFrameRate) {
			char b[4];
			sprintf(b, "%d", (int) (1.0f / localTimeStep));
			graphics.drawText(0, 0, 60, 30, b);
        }

        if (_gameState.top() == GameState::TEXTBOX) {
            draw_textbox(&graphics, &textBox, entities_get_body(&scene, 0), localTimeStep);
        }

        if (_gameState.top() == GameState::EDITOR) {
            editor_draw(&graphics,localTimeStep);
        }

        if (_gameState.top() == GameState::MODAL) {
            modal_draw(&graphics, &modal, localTimeStep);
        }

        if (_gameState.top() == GameState::TILE_PICKER) {
            graphics.drawTilesetPicker(&tilePicker);
        }

        graphics.present();
        SDL_Delay(1000 / Graphics::FRAME_RATE);

        if (_openTextBoxRequested) {
            _openTextBoxRequested = false;
            _gameState.push(GameState::TEXTBOX);
        } else if (_openModalRequested) {
            _openModalRequested = false;
            _gameState.push(GameState::MODAL);
        } else if (_openTilePickerRequested) {
            _openTilePickerRequested = false;
            _gameState.push(GameState::TILE_PICKER);
        } else if (_sceneSaveRequested) {
            _sceneSaveRequested = false;
            scene_save(&scene);
        } else if (_sceneLoadRequested) {
            _sceneLoadRequested = false;
            scene_load(&scene);
        }
    }
}
