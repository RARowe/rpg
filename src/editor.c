#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "enums.h"
#include "input.c"
#include "types.h"
#include "game_math.h"
#include "graphics_context.h"
#include "entities.c"

typedef struct {
    bool one;
    bool two;
    bool three;
    bool w;
    bool m;
    bool click;
    int x, y;
    bool drag;
    int startX, startY;
    bool release;
    bool cmd;
    bool del;
} EditorInput;

EditorInput input;
Body* selectedEntity = NULL;
bool wallTool = false;
bool showMouseDebug = false;

typedef enum {
    NONE,
    PRESSED,
    DOWN,
    DRAGGING,
    DRAGGING_ENTITY,
    RELEASED
} CursorState;

CursorState state;
int startX, startY;
int curX, curY;
int relX, relY;
int snapX, snapY;

void editor_handle_input(Input* i, bool* drawBackground, bool* drawMidground, bool* drawForeground, SceneData* s) {
    memset(&input, 0, sizeof(EditorInput));
    input.one = input_is_pressed(i, SDLK_1);
    input.two = input_is_pressed(i, SDLK_2);
    input.three = input_is_pressed(i, SDLK_3);
    input.w = input_is_pressed(i, SDLK_w);
    input.m = input_is_pressed(i, SDLK_m);
    input.cmd = input_is_down(i, SDLK_LGUI) || input_is_down(i, SDLK_RGUI);
    input.del = input_is_pressed(i, SDLK_DELETE) || input_is_pressed(i, SDLK_BACKSPACE);

    if (input.m) {
        showMouseDebug = !showMouseDebug;
    }

    if (state == RELEASED) {
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
        state = RELEASED;
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

    if (input.one) { *drawBackground = !*drawBackground; }
    if (input.two) { *drawMidground = !*drawMidground; }
    if (input.three) { *drawForeground = !*drawForeground; }
    if (input.w) { wallTool = !wallTool; }
    if (input.del && selectedEntity) {
        entities_wall_remove(s, selectedEntity);
        selectedEntity = NULL;
    }

    if (state == PRESSED) {
        for (unsigned int i = 0; i < s->bodies.size(); i++) {
            Body* b = &s->bodies[i];

            if (point_in_body(*b, startX, startY)) {
                selectedEntity = selectedEntity == b ? NULL : b;
            }
        }
    }

    if (state == RELEASED) {
        if (wallTool && (distance(startX, startY, curX, curY) > 2)) {
            int x1 = startX, y1 = startY;
            // TODO: Checking state everywhere for edge cases probably won't scale
            int x2 = input.cmd ? snapX : curX, y2 = input.cmd ? snapY : curY;
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
        if (input.cmd) {
            selectedEntity->x = (curX / 32) * 32;
            selectedEntity->y = (curY / 32) * 32;
        } else {
            selectedEntity->x = curX - relX;
            selectedEntity->y = curY - relY;
        }
    }

    if (state == DRAGGING && input.cmd) {
        startX = (startX / 32) * 32;
        startY = (startY / 32) * 32;
        snapX = (curX / 32) * 32 + 32;
        snapY = (curY / 32) * 32 + 32;
    }
}

void editor_draw(GraphicsContext* g) {
    g->drawGridOverlay();

    // Dividing then multiplying works, because there is a loss of info
    // when the int is floored.
    // This may be useful for tiles, but I'm not sure
    //g->drawBox((input.x / 32) * 32, (input.y / 32) * 32, 31, 31, Color::BLUE, 100);
    if (state == DRAGGING && input.cmd) {
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
    if (input.cmd) {
        g->drawText(0, 0, 128, 32, "CMD");
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
            case RELEASED:
                g->drawText(0, 0, 128, 32, "RELEASED");
                break;
        }
    }
}
