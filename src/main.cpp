#include "game.h"
#include "editor.c"
#include "modal.c"
#include "state_stack.c"
#include "scene.c"

static void
game_request_open_text_box(GameData* d, unsigned int textureId, const char* text) {
    d->openTextBoxRequested = true;
    d->textBox.textureId = textureId;
    d->textBox.text = text;
}

static void
game_request_open_modal(GameData* d, char** options, int numberOfOptions, int* result) {
    d->openModalRequested = true;
    
    for (int i = 0; i < numberOfOptions; i++) {
        d->modal.options[i] = options[i];
    }

    d->modal.dim.x = 31;
    d->modal.dim.y = 31;
    d->modal.dim.w = SCREEN_WIDTH - 64;
    d->modal.dim.h = SCREEN_HEIGHT - 64;

    d->modal.textStartingPoint.x = 96;
    d->modal.textStartingPoint.y = 64;

    d->modal.hasTitle = false;

    d->modal.numberOfOptions = numberOfOptions;
    d->modal.result = result;
}

static void
game_request_open_modal(GameData* d, const Body* size, const Point* textStartingPoint, const char* title, char** options, int numberOfOptions, int* result) {
    d->openModalRequested = true;
    
    for (int i = 0; i < numberOfOptions; i++) {
        d->modal.options[i] = options[i];
    }

    memcpy(&d->modal.dim, size, sizeof(Body));
    memcpy(&d->modal.textStartingPoint, textStartingPoint, sizeof(Body));

    d->modal.hasTitle = true;
    strcpy(d->modal.title, title);

    d->modal.numberOfOptions = numberOfOptions;
    d->modal.result = result;
}

static void
game_request_open_tile_picker(GameData* d, int* tile) {
    d->openTilePickerRequested = true;
    d->tile = tile;
}

static void
game_request_open_text_editor(GameData* d, char* buffer) {
    d->openTextEditorRequested = true;
    d->textEditor.outBuffer = buffer;

    size_t buflen = strlen(buffer);
    if (buflen > 0) {
        d->textEditor.cursorPos = buflen;
        strcpy(d->textEditor.buffer, buffer);
    }
}

static void
game_request_scene_save(GameData* d) {
    d->sceneSaveRequested = true;
}

static void
game_request_scene_load(GameData* d) {
    d->sceneLoadRequested = true;
}

char* mainMenu[] = {
    "Continue",
    "New Game",
    "Options"
};

int mainMenuResult;

static void
game_init(GameData* d) {
    memset(&input, 0, sizeof(PlayerInput));
    state_stack_push(&gameState, GAME_STATE_NORMAL);
    //tilePicker.tilesetMeta.id = 0;
    //tilePicker.tilesetMeta.hTiles = ((g->textureCache[0].w - 16) / 17) + 1;
    //tilePicker.tilesetMeta.vTiles = ((g->textureCache[0].h - 16) / 17) + 1;
    //tilePicker.tilesetMeta.totalTiles =
    //    tilePicker.tilesetMeta.hTiles * tilePicker.tilesetMeta.vTiles;
    //tilePicker.tile = 0;
    editor.isInitialzed = false;

    /* TODO: Remove this */
    for (int j = 0; j < scene.backgroundSize; j++) {
        scene.background[j] = -1;
        scene.midground[j] = -1;
        scene.foreground[j] = -1;
    }
    scene.spawnPoint.x = -1;
    scene.spawnPoint.y = -1;
    scene_load(&scene);
    Body b = { scene.spawnPoint.x, scene.spawnPoint.y, 32, 32 };
    scene.bodies[0] = b;
    scene.vel.xVel = 0;
    scene.vel.yVel = 0;
    /* End */
    
    /* Show opening credits */
    state_stack_push(&g->gameState, GAME_STATE_STARTUP);
}

void
game_run_frame(GameData* d, Graphics* g, Audio* a, Input* i, float timeStep) {
    if (!d->isInitialzed) {
        game_init(d);
    }

    /* Handle Input */
    if (input.r) {
        _showFrameRate = !_showFrameRate;
    }

    if (input.e && state_stack_peek(&g->gameState) == GAME_STATE_NORMAL) {
        state_stack_push(&g->gameState, GAME_STATE_EDITOR);
    }

    if (input.esc && state_stack_peek(&g->gameState) == GAME_STATE_EDITOR) {
        state_stack_pop(&g->gameState);
    }

    switch (state_stack_peek(&g->gameState)) {
        case GAME_STATE_STARTUP:
            if (input.select) {
                audio_queue_sound(audio, 0);
                d->startSelected = true;
            }

            if (input.back) {
                state_stack_pop(&g->gameState);
                audio_request_stop_music(audio);
            }

            if (d->startSelectedFlash > 2.0f) {
                state_stack_pop(&g->gameState);
                Body m_size = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
                Point textStartingPoint = { 224.0f, 128.0f };
                const char* menuTitle = "Main Menu";
                requestOpenModal
                    (
                     &m_size,
                     &textStartingPoint,
                     menuTitle,
                     mainMenu,
                     3,
                     &mainMenuResult
                    );
                audio_request_stop_music(audio);
            }
            break;
        case GAME_STATE_EDITOR:
            editor_handle_input(this, &editor, g, &i, &scene);
            break;
        case GAME_STATE_TEXTBOX:
            if (input.select) {
                state_stack_pop(&g->gameState);
            }
            break;
        case GAME_STATE_MODAL:
            if (modal_handle_input(&i, &modal)) {
                state_stack_pop(&g->gameState);
            }
            break;
        case GAME_STATE_TILE_PICKER:
            if (tile_picker_handle_input(&i, &tilePicker)) {
                *_tile = tilePicker.tile;
                state_stack_pop(&g->gameState);
            }
            break;
        case GAME_STATE_TEXT_EDITOR:
            if (text_editor_handle_input(&textEditor, &i)) {
                state_stack_pop(&g->gameState);
            }
        case GAME_STATE_NORMAL:
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
    /* End */

    if (state_stack_peek(&g->gameState) == GAME_STATE_NORMAL) {
        Body* player = entities_get_body(&scene, 0);
        float startX = player->x;
        float startY = player->y;
        player_process_movement(this, player, scene.vel, timeStep);

        for (auto&& p : scene.solidEntities) {
            Body* body = entities_get_body(&scene, p);
            if (utils_entities_collide(player, body)) {
                player->x = startX;
                player->y = startY;
                break;
            }
        }
    }

    g->drawBox(0, 0, 1000, 1000, Color::BLACK, 255);
    // Draw level
    g->drawTiles(scene.tileSet, scene.background, scene.backgroundSize);
    g->drawTiles(scene.tileSet, scene.midground, scene.midgroundSize);
    for (auto&& p : scene.tileSprites) {
        Body* body = entities_get_body(&scene, p.first);
        g->drawTile(scene.tileSet, p.second, body->x, body->y, body->w, body->h);
    }

    if (state_stack_peek(&g->gameState) == GAME_STATE_EDITOR) {
        for (auto&& p : scene.solidEntities) {
            Body* body = entities_get_body(&scene, p);
            g->drawBox(body->x, body->y, body->w, body->h, Color::WHITE, 100);
        }

        g->drawBox(scene.spawnPoint.x, scene.spawnPoint.y, 32, 32, Color::BLUE, 100);
    }
    /* Player Rendering */
    Body* b = entities_get_body(&scene, 0);
    g->drawBox(b->x, b->y, b->w, b->h, Color::BLUE, 255);
    if (foundItem > 0.0f) {
        g->drawTile(0, interactionData.item.tile, b->x, b->y - 40, 32, 32);
        foundItem -= timeStep;
    }
    g->drawTiles(scene.tileSet, scene.foreground, scene.foregroundSize);
    /* End */

    if (_showFrameRate) {
        char b[4];
        sprintf(b, "%d", (int) (1.0f / timeStep));
        g->drawText(0, 0, 60, 30, b);
    }

    if (state_stack_peek(&g->gameState) == GAME_STATE_TEXTBOX) {
        const int y = entities_get_body(&scene)->y> 256 ? 0 : 256;

        g->drawBox(0, y, 608, 160, Color::BLUE, 255);
        g->drawTexture(textBox.textureId, 0, 0, 160, 160);
        g->drawWrappedText(192, y, 32, 384, textBox.text);
    }

    if (state_stack_peek(&g->gameState) == GAME_STATE_EDITOR) {
        editor_draw(&editor, g,timeStep);
    }

    if (state_stack_peek(&g->gameState) == GAME_STATE_MODAL) {
        modal_draw(g, &modal, timeStep);
    }

    if (state_stack_peek(&g->gameState) == GAME_STATE_TILE_PICKER) {
        g->drawTilesetPicker(&tilePicker);
    }

    if (state_stack_peek(&g->gameState) == GAME_STATE_TEXT_EDITOR) {
        g->drawBox(0, 0, 1000, 1000, Color::BLUE, 255);
        g->drawWrappedText(0, 0, 32, 608, textEditor.buffer);
    }

    if (state_stack_peek(&g->gameState) == GAME_STATE_STARTUP) {
        if (d->fadeIn < 6.5f) {
            g->drawTexture(1, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            g->drawBox(0, 0,
                    SCREEN_WIDTH,
                    SCREEN_HEIGHT,
                    Color::BLACK,
                    (int)(255.0f - ((255.0f / 6.5f) * d->fadeIn)));
            if (d->fadeIn > 6.0f) {
                g->drawBox(0, 0,
                        SCREEN_WIDTH,
                        SCREEN_HEIGHT,
                        Color::WHITE,
                        (int)((255.0f / 0.5f) * (d->fadeIn - 6.0f)));
            }
        } else {
            g->drawTexture(1, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            g->drawText(SCREEN_WIDTH / 2 - 32, 64, 48, "Vulfy Story");

            float junk;
            if (d->startSelected && (modff(d->startSelectedFlash / 0.25, &junk) < 0.5f)) {
                g->drawText(SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 + 96, 48, "Press Start");
            } else if (!d->startSelected && (modff(d->flashingButton, &junk) < 0.5f)) {
                g->drawText(SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 + 96, 48, "Press Start");
            }
        }

        if (d->startSelected) {
            d->startSelectedFlash += timeStep;
        }
        d->flashingButton += timeStep;
        d->fadeIn += timeStep;
    }

    if (_openTextBoxRequested) {
        _openTextBoxRequested = false;
        state_stack_push(&g->gameState, GAME_STATE_TEXTBOX);
    } else if (_openModalRequested) {
        _openModalRequested = false;
        state_stack_push(&g->gameState, GAME_STATE_MODAL);
    } else if (_openTilePickerRequested) {
        _openTilePickerRequested = false;
        state_stack_push(&g->gameState, GAME_STATE_TILE_PICKER);
    } else if (_openTextEditorRequested) {
        _openTextEditorRequested = false;
        state_stack_push(&g->gameState, GAME_STATE_TEXT_EDITOR);
    } else if (_sceneSaveRequested) {
        _sceneSaveRequested = false;
        scene_save(&scene);
    } else if (_sceneLoadRequested) {
        _sceneLoadRequested = false;
        scene_load(&scene);
    }
}
