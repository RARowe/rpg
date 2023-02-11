#include "game.h"
#include "game_internal.h"
#include "state_stack.c"

// #include "editor.c"
// #include "entities.c"
// #include "modal.c"
// #include "scene.c"
// #include "state_stack.c"

int game_init(GameData *d) {
    state_stack_init(&d->gameState);
    state_stack_push(&d->gameState, GAME_STATE_NORMAL);
    // // TODO: Reimplement
    // // tilePicker.tilesetMeta.id = 0;
    // // tilePicker.tilesetMeta.hTiles = ((g->textureCache[0].w - 16) / 17) +
    // 1;
    // // tilePicker.tilesetMeta.vTiles = ((g->textureCache[0].h - 16) / 17) +
    // 1;
    // // tilePicker.tilesetMeta.totalTiles =
    // //    tilePicker.tilesetMeta.hTiles * tilePicker.tilesetMeta.vTiles;
    // // tilePicker.tile = 0;
    // d->editor.isInitialzed = false;

    // /* TODO: Remove this */
    // for (int j = 0; j < d->scene.backgroundSize; j++) {
    //     d->scene.background[j] = -1;
    //     d->scene.midground[j] = -1;
    //     d->scene.foreground[j] = -1;
    // }
    // d->scene.spawnPoint.x = -1;
    // d->scene.spawnPoint.y = -1;
    // scene_load(&d->scene);
    // Body b = {d->scene.spawnPoint.x, d->scene.spawnPoint.y, 32, 32};
    // d->scene.bodies[0] = b;
    // d->scene.vel.xVel = 0;
    // d->scene.vel.yVel = 0;
    // /* End */

    // /* Show opening credits */
    // state_stack_push(&d->gameState, GAME_STATE_STARTUP);
    return 0;
}

size_t game_size_of_game_data() { return sizeof(GameData); }

#include <stdlib.h>
void game_run_frame(GameData *d, Graphics *g, Audio *a, Input *i,
                    float timeStep) {
    // /* Handle Input */
    // if (input_key_is_pressed(i, KEY_r)) {
    //     d->showFrameRate = !d->showFrameRate;
    // }

    // if (input_key_is_pressed(i, KEY_e) &&
    //     state_stack_peek(&d->gameState) == GAME_STATE_NORMAL) {
    //     state_stack_push(&d->gameState, GAME_STATE_EDITOR);
    // }

    // if (input_is_pressed(i, GAME_INPUT_ESC) &&
    //     state_stack_peek(&d->gameState) == GAME_STATE_EDITOR) {
    //     state_stack_pop(&d->gameState);
    // }

    // switch (state_stack_peek(&d->gameState)) {
    // case GAME_STATE_STARTUP:
    //     if (input_is_pressed(i, GAME_INPUT_SELECT)) {
    //         audio_queue_sound(a, 0);
    //         d->startSelected = true;
    //     }

    //     if (input_is_pressed(i, GAME_INPUT_BACK)) {
    //         state_stack_pop(&d->gameState);
    //         audio_request_stop_music(a);
    //     }

    //     if (d->startSelectedFlash > 2.0f) {
    //         state_stack_pop(&d->gameState);
    //         Body m_size = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    //         Point textStartingPoint = {224.0f, 128.0f};
    //         const char *menuTitle = "Main Menu";
    //         game_request_open_modal(d, &m_size, &textStartingPoint,
    //         menuTitle,
    //                                 mainMenu, 3, &mainMenuResult);
    //         audio_request_stop_music(a);
    //     }
    //     break;
    // case GAME_STATE_EDITOR:
    //     editor_handle_input(&d->editor, d, g, i, &d->scene);
    //     break;
    // case GAME_STATE_TEXTBOX:
    //     if (input_is_pressed(i, GAME_INPUT_SELECT)) {
    //         state_stack_pop(&d->gameState);
    //     }
    //     break;
    // case GAME_STATE_MODAL:
    //     if (modal_handle_input(i, &d->modal)) {
    //         state_stack_pop(&d->gameState);
    //     }
    //     break;
    // case GAME_STATE_TILE_PICKER:
    //     if (tile_picker_handle_input(i, &d->tilePicker)) {
    //         *d->tile = d->tilePicker.tile;
    //         state_stack_pop(&d->gameState);
    //     }
    //     break;
    // case GAME_STATE_TEXT_EDITOR:
    //     if (text_editor_handle_input(&d->textEditor, i)) {
    //         state_stack_pop(&d->gameState);
    //     }
    // case GAME_STATE_NORMAL:
    // default:
    //     const int MAX_VELOCITY = 4;
    //     if (input_is_down(i, GAME_INPUT_LEFT)) {
    //         d->scene.vel.xVel = -MAX_VELOCITY;
    //     }
    //     if (input_is_down(i, GAME_INPUT_RIGHT)) {
    //         d->scene.vel.xVel = MAX_VELOCITY;
    //     }
    //     if (input_is_down(i, GAME_INPUT_UP)) {
    //         d->scene.vel.yVel = -MAX_VELOCITY;
    //     }
    //     if (input_is_down(i, GAME_INPUT_DOWN)) {
    //         d->scene.vel.yVel = MAX_VELOCITY;
    //     }

    //     scene_process_interaction(d, &d->scene, i);
    //     break;
    // }
    // /* End */

    // if (state_stack_peek(&d->gameState) == GAME_STATE_NORMAL) {
    //     Body *player = entities_get_body(&d->scene, 0);
    //     float startX = player->x;
    //     float startY = player->y;
    //     player_process_movement(player, &d->scene.vel, timeStep);

    //     for (auto &&p : d->scene.solidEntities) {
    //         Body *body = entities_get_body(&d->scene, p);
    //         if (utils_entities_collide(player, body)) {
    //             player->x = startX;
    //             player->y = startY;
    //             break;
    //         }
    //     }
    // }

    // graphics_draw_box(g, 0, 0, 1000, 1000, COLOR_BLACK, 255);
    // // Draw level
    // graphics_draw_tiles(g, d->scene.tileSet, d->scene.background,
    //                     d->scene.backgroundSize);
    // graphics_draw_tiles(g, d->scene.tileSet, d->scene.midground,
    //                     d->scene.midgroundSize);
    // for (auto &&p : d->scene.tileSprites) {
    //     Body *body = entities_get_body(&d->scene, p.first);
    //     graphics_draw_tile(g, d->scene.tileSet, p.second, body->x, body->y,
    //                        body->w, body->h);
    // }

    // if (state_stack_peek(&d->gameState) == GAME_STATE_EDITOR) {
    //     for (auto &&p : d->scene.solidEntities) {
    //         Body *body = entities_get_body(&d->scene, p);
    //         graphics_draw_box(g, body->x, body->y, body->w, body->h,
    //                           COLOR_WHITE, 100);
    //     }

    //     graphics_draw_box(g, d->scene.spawnPoint.x, d->scene.spawnPoint.y,
    //     32,
    //                       32, COLOR_BLUE, 100);
    // }
    // /* Player Rendering */
    // Body *b = entities_get_body(&d->scene, 0);
    // graphics_draw_box(g, b->x, b->y, b->w, b->h, COLOR_BLUE, 255);
    // if (d->foundItem > 0.0f) {
    //     graphics_draw_tile(g, 0, d->interactionData.item.tile, b->x, b->y -
    //     40,
    //                        32, 32);
    //     d->foundItem -= timeStep;
    // }
    // graphics_draw_tiles(g, d->scene.tileSet, d->scene.foreground,
    //                     d->scene.foregroundSize);
    // /* End */

    // if (d->showFrameRate) {
    //     char b[4];
    //     sprintf(b, "%d", (int)(1.0f / timeStep));
    //     graphics_draw_text(g, 0, 0, 60, 30, b);
    // }

    // if (state_stack_peek(&d->gameState) == GAME_STATE_TEXTBOX) {
    //     const int y = b->y > 256 ? 0 : 256;

    //     graphics_draw_box(g, 0, y, 608, 160, COLOR_BLUE, 255);
    //     graphics_draw_texture(g, d->textBox.textureId, 0, 0, 160, 160);
    //     graphics_draw_wrapped_text(g, 192, y, 32, 384,
    //     d->textBox.text.c_str());
    // }

    // if (state_stack_peek(&d->gameState) == GAME_STATE_EDITOR) {
    //     editor_draw(&d->editor, g, timeStep);
    // }

    // if (state_stack_peek(&d->gameState) == GAME_STATE_MODAL) {
    //     modal_draw(g, &d->modal, timeStep);
    // }

    // if (state_stack_peek(&d->gameState) == GAME_STATE_TILE_PICKER) {
    //     tile_picker_draw(g, &d->tilePicker);
    // }

    // if (state_stack_peek(&d->gameState) == GAME_STATE_TEXT_EDITOR) {
    //     graphics_draw_box(g, 0, 0, 1000, 1000, COLOR_BLUE, 255);
    //     graphics_draw_wrapped_text(g, 0, 0, 32, 608, d->textEditor.buffer);
    // }

    // if (state_stack_peek(&d->gameState) == GAME_STATE_STARTUP) {
    //     if (d->fadeIn < 6.5f) {
    //         graphics_draw_texture(g, 1, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //         graphics_draw_box(g, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
    //         COLOR_BLACK,
    //                           (int)(255.0f - ((255.0f / 6.5f) * d->fadeIn)));
    //         if (d->fadeIn > 6.0f) {
    //             graphics_draw_box(g, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
    //                               COLOR_WHITE,
    //                               (int)((255.0f / 0.5f) * (d->fadeIn
    //                               - 6.0f)));
    //         }
    //     } else {
    //         graphics_draw_texture(g, 1, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //         graphics_draw_text(g, SCREEN_WIDTH / 2 - 32, 64, 48, "Vulfy
    //         Story");

    //         float junk;
    //         if (d->startSelected &&
    //             (modff(d->startSelectedFlash / 0.25, &junk) < 0.5f)) {
    //             graphics_draw_text(g, SCREEN_WIDTH / 2 - 32,
    //                                SCREEN_HEIGHT / 2 + 96, 48, "Press
    //                                Start");
    //         } else if (!d->startSelected &&
    //                    (modff(d->flashingButton, &junk) < 0.5f)) {
    //             graphics_draw_text(g, SCREEN_WIDTH / 2 - 32,
    //                                SCREEN_HEIGHT / 2 + 96, 48, "Press
    //                                Start");
    //         }
    //     }

    //     if (d->startSelected) {
    //         d->startSelectedFlash += timeStep;
    //     }
    //     d->flashingButton += timeStep;
    //     d->fadeIn += timeStep;
    // }

    // if (d->openTextBoxRequested) {
    //     d->openTextBoxRequested = false;
    //     state_stack_push(&d->gameState, GAME_STATE_TEXTBOX);
    // } else if (d->openModalRequested) {
    //     d->openModalRequested = false;
    //     state_stack_push(&d->gameState, GAME_STATE_MODAL);
    // } else if (d->openTilePickerRequested) {
    //     d->openTilePickerRequested = false;
    //     state_stack_push(&d->gameState, GAME_STATE_TILE_PICKER);
    // } else if (d->openTextEditorRequested) {
    //     d->openTextEditorRequested = false;
    //     state_stack_push(&d->gameState, GAME_STATE_TEXT_EDITOR);
    // } else if (d->sceneSaveRequested) {
    //     d->sceneSaveRequested = false;
    //     scene_save(&d->scene);
    // } else if (d->sceneLoadRequested) {
    //     d->sceneLoadRequested = false;
    //     scene_load(&d->scene);
    // }
}

// static void player_process_movement(Body *body, Velocity *vel,
// const float timeStep);
//
// static void game_request_open_text_box(GameData *d, unsigned int textureId,
//    const char *text) {
// d->openTextBoxRequested = true;
// d->textBox.textureId = textureId;
// d->textBox.text = text;
// }
//
// static void game_request_open_modal(GameData *d, char **options,
// int numberOfOptions, int *result) {
// d->openModalRequested = true;
//
// for (int i = 0; i < numberOfOptions; i++) {
// d->modal.options[i] = options[i];
// }
//
// d->modal.dim.x = 31;
// d->modal.dim.y = 31;
// d->modal.dim.w = SCREEN_WIDTH - 64;
// d->modal.dim.h = SCREEN_HEIGHT - 64;
//
// d->modal.textStartingPoint.x = 96;
// d->modal.textStartingPoint.y = 64;
//
// d->modal.hasTitle = false;
//
// d->modal.numberOfOptions = numberOfOptions;
// d->modal.result = result;
// }
//
// static void game_request_open_modal(GameData *d, const Body *size,
// const Point *textStartingPoint,
// const char *title, char **options,
// int numberOfOptions, int *result) {
// d->openModalRequested = true;
//
// for (int i = 0; i < numberOfOptions; i++) {
// d->modal.options[i] = options[i];
// }
//
// memcpy(&d->modal.dim, size, sizeof(Body));
// memcpy(&d->modal.textStartingPoint, textStartingPoint, sizeof(Body));
//
// d->modal.hasTitle = true;
// strcpy(d->modal.title, title);
//
// d->modal.numberOfOptions = numberOfOptions;
// d->modal.result = result;
// }
//
// static void game_request_open_tile_picker(GameData *d, int *tile) {
// d->openTilePickerRequested = true;
// d->tile = tile;
// }
//
// static void game_request_open_text_editor(GameData *d, char *buffer) {
// d->openTextEditorRequested = true;
// d->textEditor.outBuffer = buffer;
//
// size_t buflen = strlen(buffer);
// if (buflen > 0) {
// d->textEditor.cursorPos = buflen;
// strcpy(d->textEditor.buffer, buffer);
// }
// }
//
// static void game_request_scene_save(GameData *d) {
// d->sceneSaveRequested = true;
// }
//
// static void game_request_scene_load(GameData *d) {
// d->sceneLoadRequested = true;
// }
//
// char *mainMenu[] = {"Continue", "New Game", "Options"};
//
// int mainMenuResult;
//
// static void player_process_movement(Body *body, Velocity *vel,
// const float timeStep) {
// float startX = body->x;
// float startY = body->y;
// int xVelocity = vel->xVel;
// int yVelocity = vel->yVel;
// if (xVelocity < 0) {
// body->x += -120 * timeStep;
// vel->xVel += 2;
// } else if (xVelocity > 0) {
// body->x += 120 * timeStep;
// vel->xVel += -2;
// }
//
// if (yVelocity < 0) {
// body->y += -120 * timeStep;
// vel->yVel += 2;
// } else if (yVelocity > 0) {
// body->y += 120 * timeStep;
// vel->yVel += -2;
// }
//
// if (startX < -30) {
// body->x = SCREEN_WIDTH + 30;
// } else if (startX > SCREEN_WIDTH + 30) {
// body->x = -30;
// }
//
// if (startY < -30) {
// body->y = SCREEN_HEIGHT + 30;
// } else if (startY > SCREEN_HEIGHT + 30) {
// body->y = -30;
// }
// }
//