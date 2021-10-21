#include <SDL2/SDL.h>
#include <string.h>
#include "enums.h"
#include "input.c"
#include "types.h"
#include "game_context.h"
#include "game_math.h"
#include "graphics_context.h"
#include "entities.c"

typedef enum {
    EDITOR_MODE_OBJECT,
    EDITOR_MODE_TILE,
    EDITOR_MODE_TEXT_EDIT,
    EDITOR_MODE_TILE_SELECT
} EditorMode;

Body* selectedEntity = NULL;
bool wallTool = false;
bool showMouseDebug = false;
bool showGrid = true;
bool openTexture = false;
int cursorIndex = 0;
EditorMode requestedNextEditorMode = EDITOR_MODE_OBJECT;
EditorMode editorMode = EDITOR_MODE_OBJECT;

typedef enum {
    NONE,
    PRESSED,
    DOWN,
    DRAGGING,
    DRAGGING_ENTITY,
    RELEASED_ENTITY,
    RELEASED
} CursorState;

typedef enum {
    LAYER_BACKGROUND,
    LAYER_MIDGROUND,
    LAYER_FOREGROUND
} Layer;

typedef struct {
    int tile;
    int x, y;
    Layer layer;
} TileEditor;

TileEditor tileEditorState = { 0, 0, LAYER_BACKGROUND };

CursorState state;
int startX, startY;
int curX, curY;
int relX, relY;
int snapX, snapY;

bool cmdDown = false;

// Text Editor Data
char textEditorBuffer[1024] = { 0 };
int textEditorCursorPosition = 0;
//

char* options[] = {
    "Text Interaction",
    "Find Item"
};
int result;

void editor_handle_input(GameContext* c, GraphicsContext* g, Input* i, SceneData* s) {
    editorMode = requestedNextEditorMode;
    if (state == RELEASED || state == RELEASED_ENTITY) {
        state = NONE;
    }

    if (state == PRESSED) {
        state = DOWN;
    }

    if (i->mouseState == MOUSE_STATE_PRESSED) {
        state = PRESSED;
        startX = i->pressedMouseCoords.x;
        startY = i->pressedMouseCoords.y;
    } else if (i->mouseState == MOUSE_STATE_RELEASED) {
        // TODO: This prevents performing a tool action if the user
        //       is dragging an entity
        state = state == DRAGGING_ENTITY ? RELEASED_ENTITY : RELEASED;
        curX = i->releasedMouseCoords.x;
        curY = i->releasedMouseCoords.y;
    }

    if (i->mouseMoving) {
        curX = i->currentMouseCoords.x;
        curY = i->currentMouseCoords.y;
        if ((state == DOWN || state == PRESSED) && (distance(startX, startY, curX, curY) > 2)) {
            if (selectedEntity && point_in_body(*selectedEntity, startX, startY)) {
                state = DRAGGING_ENTITY;
                relX = startX - selectedEntity->x;
                relY = startY - selectedEntity->y;
            } else {
                state = DRAGGING;
            }
        }
    }
    // Global Input
    if (input_is_pressed(i, SDLK_m)) {
        showMouseDebug = !showMouseDebug;
    }

    if (input_is_pressed(i, SDLK_g)) {
        showGrid = !showGrid;
    }
    // End
    
    if (editorMode == EDITOR_MODE_OBJECT) {
        cmdDown = input_is_down(i, SDLK_LGUI) || input_is_down(i, SDLK_RGUI);

        if (input_is_pressed(i, SDLK_w)) { wallTool = !wallTool; }
        if (input_is_pressed(i, SDLK_o)) { openTexture = !openTexture; }

        if (input_is_pressed(i, SDLK_UP)) { 
            cursorIndex = cursorIndex - 1 < 0 ? 0 : cursorIndex - 1;
        }
        if (input_is_pressed(i, SDLK_DOWN)) {
            cursorIndex = cursorIndex + 1 == g->textureCache.size() ?
                cursorIndex :
                cursorIndex + 1;
        }
        if (input_is_pressed(i, SDLK_f) && openTexture) {
            s->tileSet = cursorIndex;
            openTexture = false;
        }

        if (input_is_pressed(i, SDLK_t)) {
            requestedNextEditorMode = EDITOR_MODE_TILE;
        }

        if (input_is_pressed(i, SDLK_d)) {
            requestedNextEditorMode = EDITOR_MODE_TEXT_EDIT;
        }

        bool deletePressed = input_is_pressed(i, SDLK_DELETE) || input_is_pressed(i, SDLK_BACKSPACE);
        if (deletePressed && selectedEntity) {
            entities_wall_remove(s, selectedEntity);
            selectedEntity = NULL;
        }

        if (state == PRESSED) {
            selectedEntity = NULL;
            for (unsigned int j = 0; j < s->bodies.size(); j++) {
                Body* b = &s->bodies[j];

                if (point_in_body(*b, startX, startY)) {
                    //selectedEntity = selectedEntity == b ? NULL : b;
                    selectedEntity = b;
                    if (i->doubleClick) {
                        c->requestOpenModal(options, 2, &result);
                        requestedNextEditorMode = EDITOR_MODE_TEXT_EDIT;
                    }
                    break;
                }
            }
        }

        if (state == RELEASED) {
            if (wallTool && (distance(startX, startY, curX, curY) > 2)) {
                int x1 = startX, y1 = startY;
                // TODO: Checking state everywhere for edge cases probably won't scale
                int x2 = cmdDown ? snapX : curX, y2 = cmdDown ? snapY : curY;
                float x = x1 <= x2 ? x1 : x2;
                float y = y1 <= y2 ? y1 : y2;
                int xp = x1 > x2 ? x1 : x2;
                int yp = y1 > y2 ? y1 : y2;
                unsigned short w = xp - x;
                unsigned short h = yp - y;
                entities_wall_add(s, x, y, w, h);
            }
        }

        if (state == DRAGGING_ENTITY) {
            if (cmdDown) {
                selectedEntity->x = (curX / 32) * 32;
                selectedEntity->y = (curY / 32) * 32;
            } else {
                selectedEntity->x = curX - relX;
                selectedEntity->y = curY - relY;
            }
        }

        if (state == DRAGGING && cmdDown) {
            startX = (startX / 32) * 32;
            startY = (startY / 32) * 32;
            snapX = (curX / 32) * 32 + 32;
            snapY = (curY / 32) * 32 + 32;
        }
    } else if (editorMode == EDITOR_MODE_TEXT_EDIT) {
        // TODO: Text editor interface should be something like text_edit_open(char* buffer, int size)
        char c;
        if (input_get_last_pressed_key(i, &c)) {
            if (c == '\r') {
                requestedNextEditorMode = EDITOR_MODE_OBJECT;
                entities_text_interaction_add(s, selectedEntity, textEditorBuffer);
                textEditorBuffer[0] = '\0';
                textEditorCursorPosition = 0;
            } else if (c == '\b') {
                textEditorBuffer[--textEditorCursorPosition] = '\0';
                textEditorCursorPosition = textEditorCursorPosition < 0 ? 0 : textEditorCursorPosition;
            } else {
                textEditorBuffer[textEditorCursorPosition++] = c;
            }
        }
    } else if (editorMode == EDITOR_MODE_TILE_SELECT) {
        requestedNextEditorMode = EDITOR_MODE_TILE;
    } else {
        if (input_is_pressed(i, SDLK_t)) {
            requestedNextEditorMode = EDITOR_MODE_OBJECT;
        }

        if (input_is_pressed(i, SDLK_s)) {
            requestedNextEditorMode = EDITOR_MODE_TILE_SELECT;
            c->requestOpenTilePicker(1, &tileEditorState.tile);
        }

        if (input_is_pressed(i, SDLK_SPACE)) {
            if (tileEditorState.layer == LAYER_BACKGROUND) {
                tileEditorState.layer = LAYER_MIDGROUND;
            } else if (tileEditorState.layer == LAYER_MIDGROUND) {
                tileEditorState.layer = LAYER_FOREGROUND;
            } else {
                tileEditorState.layer = LAYER_BACKGROUND;
            }
        }

        tileEditorState.x = (curX / 32) * 32;
        tileEditorState.y = (curY / 32) * 32;

        if (state == PRESSED || state == DRAGGING) {
            int col = tileEditorState.x / 32;
            int row = tileEditorState.y / 32;
            int tile = row * 19 + col;

            switch (tileEditorState.layer) {
                case LAYER_BACKGROUND:
                    s->background[tile] = tileEditorState.tile;
                    break;
                case LAYER_MIDGROUND:
                    s->midground[tile] = tileEditorState.tile;
                    break;
                case LAYER_FOREGROUND:
                    s->foreground[tile] = tileEditorState.tile;
                    break;
            }
        }
    }
}

void editor_draw(GraphicsContext* g, float timeStep) {
    if (showGrid) {
        g->drawGridOverlay();
    }

    if (editorMode == EDITOR_MODE_OBJECT) {
        if (state == DRAGGING && cmdDown) {
            g->drawSelection(startX, startY, snapX, snapY);
        } else if (state == DRAGGING) {
            g->drawSelection(startX, startY, curX, curY);
        }

        if (selectedEntity) {
            g->drawBox(selectedEntity->x, selectedEntity->y, selectedEntity->w, selectedEntity->h, Color::BLUE, 100);
        }

        if (wallTool) {
            g->drawText(0, 0, 128, 32, "WALL TOOL");
        }
        if (cmdDown) {
            g->drawText(0, 0, 128, 32, "CMD");
        }

        if (openTexture) {
            g->drawBox(31, 31, g->width - 64, g->height - 64, Color::BLUE, 255);
            g->drawText(64, 64 + (32 * cursorIndex), 32, ">");

            int i = 0;
            for (auto&& t : g->textureCache) {
                g->drawText(96, 64 + i, 32, t.second.name);
                i += 32;
            }
        }

        if (showMouseDebug) {
            switch (state) {
                case NONE:
                    g->drawText(0, 0, 128, 32, "NONE");
                    break;
                case PRESSED:
                    g->drawText(0, 0, 128, 32, "PRESSED");
                    break;
                case DOWN:
                    g->drawText(0, 0, 128, 32, "DOWN");
                    break;
                case DRAGGING:
                    g->drawText(0, 0, 128, 32, "DRAGGING");
                    break;
                case DRAGGING_ENTITY:
                    g->drawText(0, 0, 128, 32, "DRAGGING ENTITY");
                    break;
                case RELEASED_ENTITY:
                    g->drawText(0, 0, 128, 32, "RELASED ENTITY");
                    break;
                case RELEASED:
                    g->drawText(0, 0, 128, 32, "RELEASED");
                    break;
            }
        }
    } else if (editorMode == EDITOR_MODE_TEXT_EDIT) {
        g->drawBox(0, 0, 1000, 1000, Color::BLUE, 255);
        g->drawWrappedText(0, 0, 32, 608, textEditorBuffer);
    } else {
        g->drawTile(1, tileEditorState.tile, tileEditorState.x, tileEditorState.y, 32, 32);
    }
}
