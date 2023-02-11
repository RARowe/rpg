#include "game.h"

#include <string.h>

char *fileMenu[] = {"Save", "Load Scene", "Open Texture"};

char *toolMenu[] = {"Select Tool", "Wall Tool", "Text Interaction",
                    "Spawn Point", "Tile Tool", "Item Tool"};

char *debugMenu[] = {"Grid Toggle"};

char *numberOfSpeakers[] = {"Two", "Three", "Four"};

char *speakers[] = {"Speaker One", "Speaker Two", "Speaker Three",
                    "Speaker Four"};

static void editor_handle_input_normal(Editor *e, GameData *d, Graphics *g,
                                       Input *i, SceneData *s) {
    if (input_is(i->mouseState, INPUT_STATE_DOWN)) {
        state_stack_push(&e->mode, EDITOR_MODE_EDIT);
        e->currentTool = TOOL_SELECT;
    }

    if (input_key_is_pressed(i, KEY_f)) {
        e->toolBarState = e->toolBarState == TOOLBAR_STATE_FILE
                              ? TOOLBAR_STATE_DEFAULT
                              : TOOLBAR_STATE_FILE;
    } else if (input_key_is_pressed(i, KEY_t)) {
        e->toolBarState = e->toolBarState == TOOLBAR_STATE_TOOL
                              ? TOOLBAR_STATE_DEFAULT
                              : TOOLBAR_STATE_TOOL;
    } else if (input_key_is_pressed(i, KEY_d)) {
        e->toolBarState = e->toolBarState == TOOLBAR_STATE_DEBUG
                              ? TOOLBAR_STATE_DEFAULT
                              : TOOLBAR_STATE_DEBUG;
    }

    switch (e->toolBarState) {
    case TOOLBAR_STATE_FILE:
        if (input_key_is_pressed(i, KEY_s)) {
            strcpy(s->name, "test.level");
            game_request_scene_save(d);
            e->toolBarState = TOOLBAR_STATE_DEFAULT;
        }

        if (input_key_is_pressed(i, KEY_l)) {
            strcpy(s->name, "test.level");
            game_request_scene_load(d);
            e->toolBarState = TOOLBAR_STATE_DEFAULT;
        }
        break;
    case TOOLBAR_STATE_TOOL:
        if (input_key_is_pressed(i, KEY_s)) {
            e->currentTool = TOOL_SELECT;
            e->toolBarState = TOOLBAR_STATE_DEFAULT;
            state_stack_push(&e->mode, EDITOR_MODE_EDIT);
        }

        if (input_key_is_pressed(i, KEY_w)) {
            e->currentTool = TOOL_WALL;
            e->toolBarState = TOOLBAR_STATE_DEFAULT;
            state_stack_push(&e->mode, EDITOR_MODE_EDIT);
        }

        if (input_key_is_pressed(i, KEY_e)) {
            e->currentTool = TOOL_TEXT_INTERACTION;
            e->toolBarState = TOOLBAR_STATE_DEFAULT;
            state_stack_push(&e->mode, EDITOR_MODE_EDIT);
        }

        if (input_key_is_pressed(i, KEY_p)) {
            e->currentTool = TOOL_SPAWN_POINT;
            e->toolBarState = TOOLBAR_STATE_DEFAULT;
            state_stack_push(&e->mode, EDITOR_MODE_EDIT);
        }

        if (input_key_is_pressed(i, KEY_i)) {
            e->currentTool = TOOL_TILE;
            e->toolBarState = TOOLBAR_STATE_DEFAULT;
            state_stack_push(&e->mode, EDITOR_MODE_EDIT);
        }

        if (input_key_is_pressed(i, KEY_m)) {
            e->currentTool = TOOL_ITEM;
            e->toolBarState = TOOLBAR_STATE_DEFAULT;
            state_stack_push(&e->mode, EDITOR_MODE_EDIT);
        }

        break;
    case TOOLBAR_STATE_DEBUG:
        if (input_key_is_pressed(i, KEY_g)) {
            e->showGrid = !e->showGrid;
        }
        break;
    default:
        break;
    }
}

static void editor_handle_input_edit(Editor *e, GameData *d, Graphics *g,
                                     Input *i, SceneData *s) {
    const bool modKey = input_is(i, GAME_INPUT_CTRL, INPUT_STATE_DOWN);
    if (modKey && input_key_is_pressed(i, KEY_q)) {
        state_stack_pop(&e->mode);
        return;
    }

    const bool deletePressed = input_key_is_pressed(i, KEY_BACKSPACE);

    switch (e->currentTool) {
    case TOOL_SELECT:
    case TOOL_WALL:
        if (deletePressed && e->selectedEntity) {
            entities_wall_remove(s, e->selectedEntity);
            e->selectedEntity = NULL;
        }

        if (e->currentTool == TOOL_WALL && !e->selectedEntity &&
            i->mouseState == INPUT_STATE_RELEASED && e->isDragging) {
            int x1 = e->startX, y1 = e->startY;
            int x2 = e->curX, y2 = e->curY;
            float x = x1 <= x2 ? x1 : x2;
            float y = y1 <= y2 ? y1 : y2;
            int xp = x1 > x2 ? x1 : x2;
            int yp = y1 > y2 ? y1 : y2;
            unsigned short w = xp - x;
            unsigned short h = yp - y;
            entities_wall_add(s, x, y, w, h);
        }
        break;
    case TOOL_TEXT_INTERACTION:
        if (e->selectedEntity) {
            state_stack_push(&e->mode, EDITOR_MODE_REQUEST_RESOURCE);

            /* Check if text interaction exists. If not, clear buffer */
            int size = entities_text_interaction_get(s, e->selectedEntity,
                                                     e->textBuffer);
            if (!size) {
                bzero(e->textBuffer, sizeof(char) * 1024);
            }
            game_request_open_text_editor(d, e->textBuffer);
        }
        break;
    case TOOL_SPAWN_POINT:
        if (i->mouseState == INPUT_STATE_PRESSED) {
            /* Clamp spawn to nearest grid point */
            entities_spawn_point_set(s, (e->curX / 32) * 32,
                                     (e->curY / 32) * 32);
        }
        break;
    case TOOL_TILE:
        if (input_key_is_pressed(i, KEY_s)) {
            game_request_open_tile_picker(d, &e->tileEditor.tile);
        }

        if (input_key_is_pressed(i, KEY_SPACE)) {
            if (e->tileEditor.layer == LAYER_BACKGROUND) {
                e->tileEditor.layer = LAYER_MIDGROUND;
            } else if (e->tileEditor.layer == LAYER_MIDGROUND) {
                e->tileEditor.layer = LAYER_FOREGROUND;
            } else {
                e->tileEditor.layer = LAYER_BACKGROUND;
            }
        }
        e->tileEditor.x = (e->curX / 32) * 32;
        e->tileEditor.y = (e->curY / 32) * 32;

        if (i->mouseState == INPUT_STATE_PRESSED ||
            i->mouseState == INPUT_STATE_DOWN) {
            int col = e->tileEditor.x / 32;
            int row = e->tileEditor.y / 32;
            int tile = row * 19 + col;

            switch (e->tileEditor.layer) {
            case LAYER_BACKGROUND:
                s->background[tile] = e->tileEditor.tile;
                break;
            case LAYER_MIDGROUND:
                s->midground[tile] = e->tileEditor.tile;
                break;
            case LAYER_FOREGROUND:
                s->foreground[tile] = e->tileEditor.tile;
                break;
            }
        }
        break;
    case TOOL_ITEM:
        if (e->selectedEntity) {
            state_stack_push(&e->mode, EDITOR_MODE_REQUEST_RESOURCE);
            bzero(e->textBuffer, sizeof(char) * 1024);
            game_request_open_text_editor(d, e->textBuffer);
            game_request_open_tile_picker(d, &e->result);
        }
        break;
    default:
        break;
    }
}

static void editor_handle_input(Editor *e, GameData *d, Graphics *g, Input *i,
                                SceneData *s) {
    /* Init editor if not initialized */
    if (!e->isInitialzed) {
        e->isInitialzed = true;
        state_stack_init(&e->mode);
        e->selectedEntity = NULL;
        e->toolBarState = TOOLBAR_STATE_DEFAULT;
        e->tileEditor = {0, 0, LAYER_BACKGROUND};
    }

    /* Set current mode on editor */
    const EditorMode mode = (EditorMode)state_stack_peek(&e->mode);
    e->currentMode = mode < 0 ? EDITOR_MODE_NORMAL : mode;

    /* Handle cursor information from input */
    e->curX = i->mouseX;
    e->curY = i->mouseY;

    switch (i->mouseState) {
    case INPUT_STATE_PRESSED:
        e->startX = i->mouseX;
        e->startY = i->mouseY;
        e->toolBarState = TOOLBAR_STATE_DEFAULT;

        /* Check to see if entity is clicked on */
        e->selectedEntity = NULL;
        for (int j = 0; j < s->bodies.size(); j++) {
            Body *b = &s->bodies[j];
            if (point_in_body(b, e->startX, e->startY)) {
                e->selectedEntity = b;
                e->relX = e->startX - e->selectedEntity->x;
                e->relY = e->startY - e->selectedEntity->y;
                break;
            }
        }
        break;
    case INPUT_STATE_RELEASED:
        e->curX = i->mouseX;
        e->curY = i->mouseY;
        break;
    case INPUT_STATE_UP:
        e->startX = i->mouseX;
        e->startY = i->mouseY;
        e->curX = i->mouseX;
        e->curY = i->mouseY;
        break;
    case INPUT_STATE_DOWN:
    default:
        break;
    }

    /* Modify coordinates if CMD is held down */
    const bool snapToGrid = input_is_down(i, GAME_INPUT_CMD);
    if (snapToGrid) {
        e->startX = (e->startX / 32) * 32;
        e->startY = (e->startY / 32) * 32;
        e->curX = (e->curX / 32) * 32;
        e->curY = (e->curY / 32) * 32;
    }

    /* Determine if user is dragging mouse */
    /* TODO: This may only be needed for select mode */
    e->isDragging = distance(e->startX, e->startY, e->curX, e->curY) > 2;
    if (e->isDragging && e->selectedEntity) {
        e->selectedEntity->x = e->curX - e->relX;
        e->selectedEntity->y = e->curY - e->relY;

        if (snapToGrid) {
            e->selectedEntity->x = ((int)e->selectedEntity->x / 32) * 32;
            e->selectedEntity->y = ((int)e->selectedEntity->y / 32) * 32;
        }
    }

    /* Handle general input */
    switch (e->currentMode) {
    case EDITOR_MODE_EDIT:
        editor_handle_input_edit(e, d, g, i, s);
        break;
    case EDITOR_MODE_REQUEST_RESOURCE:
        state_stack_pop(&e->mode);
        if (e->currentTool == TOOL_TEXT_INTERACTION) {
            entities_text_interaction_add(s, e->selectedEntity, e->textBuffer);
            e->selectedEntity = NULL;
        } else if (e->currentTool == TOOL_ITEM) {
            entities_item_add(s, e->selectedEntity, e->result, e->textBuffer);
            e->selectedEntity = NULL;
        }
        break;
    case EDITOR_MODE_NORMAL:
    default:
        editor_handle_input_normal(e, d, g, i, s);
        break;
    }
}

static void editor_draw(Editor *e, Graphics *g, float timeStep) {
    if (e->showGrid) {
        graphics_draw_grid_overlay(g);
    }

    if (e->currentMode == EDITOR_MODE_EDIT) {
        switch (e->currentTool) {
        case TOOL_SELECT:
        case TOOL_WALL:
            if (e->selectedEntity) {
                graphics_draw_box(g, e->selectedEntity->x, e->selectedEntity->y,
                                  e->selectedEntity->w, e->selectedEntity->h,
                                  COLOR_BLUE, 100);
            }
            if (e->isDragging && !e->selectedEntity) {
                graphics_draw_selection(g, e->startX, e->startY, e->curX,
                                        e->curY);
            }
            break;
        case TOOL_SPAWN_POINT:
            graphics_draw_box(g, (e->curX / 32) * 32, (e->curY / 32) * 32, 32,
                              32, COLOR_BLUE, 100);
            break;
        case TOOL_TILE:
            graphics_draw_tile(g, 0, e->tileEditor.tile, e->tileEditor.x,
                               e->tileEditor.y, 32, 32);
            break;
        default:
            break;
        }
    } else {
        graphics_draw_box(g, 0, 0, SCREEN_WIDTH, 24, COLOR_BLUE, 255);
        graphics_draw_text(g, 0, 0, 24, "File | Tools | Debug");

        switch (e->toolBarState) {
        case TOOLBAR_STATE_FILE:
            graphics_draw_menu(g, 0, 24, 24, fileMenu, 2);
            break;
        case TOOLBAR_STATE_TOOL:
            graphics_draw_menu(g, 66, 24, 24, toolMenu, 6);
            break;
        case TOOLBAR_STATE_DEBUG:
            graphics_draw_menu(g, 180, 24, 24, debugMenu, 1);
            break;
        default:
            break;
        }
    }
}
