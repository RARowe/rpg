#include <SDL2/SDL.h>
#include <string.h>
#include "input.c"
#include "types.h"
#include "game_context.h"
#include "graphics.h"
#include "entities.c"

typedef enum {
    EDITOR_MODE_DEFAULT = 0,
    EDITOR_MODE_OBJECT,
    EDITOR_MODE_TILE,
    EDITOR_MODE_SPAWN_POINT,
    EDITOR_MODE_TEXT_EDIT,
    EDITOR_MODE_MODAL_OPEN
} EditorMode;

void editor_input_handler_default(GameContext* c, Graphics* g, Input* i, SceneData* s);
void editor_input_handler_tile(GameContext* c, Graphics* g, Input* i, SceneData* s);
void editor_input_handler_text(GameContext* c, Graphics* g, Input* i, SceneData* s);
void editor_input_handler_modal_open(GameContext* c, Graphics* g, Input* i, SceneData* s);

void (*input_handlers[6])(GameContext*,Graphics*,Input*,SceneData*) = {
    editor_input_handler_default,
    editor_input_handler_default,
    editor_input_handler_tile,
    editor_input_handler_default,
    editor_input_handler_text,
    editor_input_handler_modal_open
};

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
char* fileMenu[] = {
    "Save",
    "Load Scene",
    "Open Texture"
};
char* toolMenu[] = {
    "Object",
    "Tiles",
    "Walls",
    "Spawn Point"
};
char* debugMenu[] = {
    "Mouse Debug",
    "Grid Toggle"
};
int result;

typedef enum {
    DEFAULT,
    FILE_MENU,
    TOOL_MENU,
    DEBUG_MENU
} ToolbarState;
ToolbarState toolBarState = DEFAULT;

void editor_input_handler_default(GameContext* c, Graphics* g, Input* i, SceneData* s) {
    if (input_is_pressed(i, SDLK_f)) {
        toolBarState = toolBarState == FILE_MENU ? DEFAULT : FILE_MENU;
    }
    if (input_is_pressed(i, SDLK_t)) {
        toolBarState = toolBarState == TOOL_MENU ? DEFAULT : TOOL_MENU;
    }
    if (input_is_pressed(i, SDLK_d)) {
        toolBarState = toolBarState == DEBUG_MENU ? DEFAULT : DEBUG_MENU;
    }

    switch (toolBarState) {
        case FILE_MENU:
            if (input_is_pressed(i, SDLK_s)) {
                strcpy(s->name, "test.level");
                c->requestSceneSave();
                toolBarState = DEFAULT;
            }

            if (input_is_pressed(i, SDLK_l)) {
                strcpy(s->name, "test.level");
                c->requestSceneLoad();
                toolBarState = DEFAULT;
            }

            if (input_is_pressed(i, SDLK_o)) {
                openTexture = !openTexture;
                requestedNextEditorMode = EDITOR_MODE_MODAL_OPEN;
            }
            break;
        case TOOL_MENU:
            if (input_is_pressed(i, SDLK_w)) {
                wallTool = !wallTool;
                toolBarState = DEFAULT;
            }

            if (input_is_pressed(i, SDLK_i)) {
                requestedNextEditorMode = EDITOR_MODE_TILE;
                toolBarState = DEFAULT;
            }

            if (input_is_pressed(i, SDLK_s)) {
                requestedNextEditorMode = EDITOR_MODE_SPAWN_POINT;
                toolBarState = DEFAULT;
            }
            break;
        case DEBUG_MENU:
            if (input_is_pressed(i, SDLK_m)) {
                showMouseDebug = !showMouseDebug;
            }

            if (input_is_pressed(i, SDLK_g)) {
                showGrid = !showGrid;
            }
            break;
        default:
            break;
    }
}

void editor_input_handler_tile(GameContext* c, Graphics* g, Input* i, SceneData* s) {
    if (input_is_pressed(i, SDLK_t)) {
        requestedNextEditorMode = EDITOR_MODE_OBJECT;
    }

    if (input_is_pressed(i, SDLK_s)) {
        c->requestOpenTilePicker(&tileEditorState.tile);
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
}

void editor_input_handler_text(GameContext* c, Graphics* g, Input* i, SceneData* s) {
    char ch;
    if (input_get_last_pressed_key(i, &ch)) {
        if (ch == '\r') {
            requestedNextEditorMode = EDITOR_MODE_OBJECT;
            entities_text_interaction_add(s, selectedEntity, textEditorBuffer);
            textEditorBuffer[0] = '\0';
            bzero(textEditorBuffer, sizeof(char) * 1024);
            textEditorCursorPosition = 0;
            selectedEntity = NULL;
        } else if (ch == '\b') {
            textEditorBuffer[--textEditorCursorPosition] = '\0';
            textEditorCursorPosition = textEditorCursorPosition < 0 ? 0 : textEditorCursorPosition;
        } else {
            textEditorBuffer[textEditorCursorPosition++] = ch;
        }
    }
}

void editor_input_handler_modal_open(GameContext* c, Graphics* g, Input* i, SceneData* s) {
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
        requestedNextEditorMode = EDITOR_MODE_OBJECT;
    }

    if (input_is_pressed(i, SDLK_d)) {
        openTexture = false;
        requestedNextEditorMode = EDITOR_MODE_OBJECT;
    }
}

void editor_handle_input(GameContext* c, Graphics* g, Input* i, SceneData* s) {
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

    /* Input handling dispatch */
    input_handlers[editorMode](c, g, i, s);

    if (editorMode == EDITOR_MODE_OBJECT) {
        cmdDown = input_is_down(i, SDLK_LGUI) || input_is_down(i, SDLK_RGUI);

        bool deletePressed =
            input_is_pressed(i, SDLK_DELETE) || input_is_pressed(i, SDLK_BACKSPACE);


        if (deletePressed && selectedEntity) {
            entities_wall_remove(s, selectedEntity);
            selectedEntity = NULL;
        }

        if (state == PRESSED) {
            selectedEntity = NULL;
            for (unsigned int j = 0; j < s->bodies.size(); j++) {
                Body* b = &s->bodies[j];

                if (point_in_body(*b, startX, startY)) {
                    selectedEntity = b;
                    if (i->doubleClick) {
                        c->requestOpenModal(options, 2, &result);
                        requestedNextEditorMode = EDITOR_MODE_TEXT_EDIT;
                        Body* se = selectedEntity;
                        textEditorCursorPosition =
                            entities_text_interaction_get(s, se, textEditorBuffer);
                        state = NONE;
                    }
                    break;
                }
            }
        }

        if (state == RELEASED) {
            if (wallTool && (distance(startX, startY, curX, curY) > 2)) {
                int x1 = startX, y1 = startY;
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
    } else if (editorMode == EDITOR_MODE_SPAWN_POINT) {
        if (state == RELEASED) {
            /* Clamp spawn to nearest grid point */
            entities_spawn_point_set(s, (curX / 32) * 32, (curY / 32) *32);
            requestedNextEditorMode = EDITOR_MODE_DEFAULT;
        }
    } else if (editorMode == EDITOR_MODE_TILE) {
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

void editor_draw(Graphics* g, float timeStep) {
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
    }

    if (editorMode == EDITOR_MODE_MODAL_OPEN && openTexture) {
        g->drawBox(31, 31, g->width - 64, g->height - 64, Color::BLUE, 255);
        g->drawText(64, 64 + (32 * cursorIndex), 32, ">");

        int i = 0;
        for (auto&& t : g->textureCache) {
            g->drawText(96, 64 + i, 32, t.second.name);
            i += 32;
        }
    }

    if (editorMode == EDITOR_MODE_TILE) {
        g->drawTile(0, tileEditorState.tile, tileEditorState.x, tileEditorState.y, 32, 32);
    }

    g->drawBox(0, 0, g->width, 24, Color::BLUE, 255);
    g->drawText(0, 0, 24, "File | Tools | Debug");

    if (showMouseDebug) {
        const char* text;
        switch (state) {
            case NONE:
                text = "NONE";
                break;
            case PRESSED:
                text = "PRESSED";
                break;
            case DOWN:
                text = "DOWN";
                break;
            case DRAGGING:
                text = "DRAGGING";
                break;
            case DRAGGING_ENTITY:
                text = "DRAGGING ENTITY";
                break;
            case RELEASED_ENTITY:
                text = "RELEASED ENTITY";
                break;
            case RELEASED:
                text = "RELEASED";
                break;
        }
        g->drawText(320, 0, 24, text);
    }

    switch (toolBarState) {
        case FILE_MENU:
            g->drawMenu(0, 24, 24, fileMenu, 3);
            break;
        case TOOL_MENU:
            g->drawMenu(66, 24, 24, toolMenu, 4);
            break;
        case DEBUG_MENU:
            g->drawMenu(180, 24, 24, debugMenu, 2);
            break;
        default:
            break;
    }

    if (editorMode == EDITOR_MODE_TEXT_EDIT) {
        g->drawBox(0, 0, 1000, 1000, Color::BLUE, 255);
        g->drawWrappedText(0, 0, 32, 608, textEditorBuffer);
    }
}
