#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "enums.h"
#include "types.h"
#include "game_math.h"
#include "graphics_context.h"

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
} MouseState;

MouseState state;
int startX, startY;
int curX, curY;
int relX, relY;

void editor_handle_input(SDL_Event* event, bool* drawBackground, bool* drawMidground, bool* drawForeground, SceneData* s) {
    //// Set all flags to false
    memset(&input, 0, sizeof(EditorInput));

    if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
        bool isKeyDown = event->type == SDL_KEYDOWN;

        switch (event->key.keysym.sym) {
            case SDLK_1:
                input.one = isKeyDown;
                break;
            case SDLK_2:
                input.two = isKeyDown;
                break;
            case SDLK_3:
                input.three = isKeyDown;
                break;
            case SDLK_w:
                input.w = isKeyDown;
                break;
            case SDLK_m:
                input.m = isKeyDown;
                break;
            default:
                break;
        }
    }

    if (input.m) {
        showMouseDebug = !showMouseDebug;
    }

    if (state == RELEASED) {
        state = NONE;
    }

    if (state == PRESSED) {
        state = DOWN;
    }

    if (event->type == SDL_MOUSEBUTTONDOWN) {
        state = PRESSED;
        startX = event->motion.x;
        startY = event->motion.y;
    } else if (event->type == SDL_MOUSEMOTION) {
        curX = event->motion.x;
        curY = event->motion.y;
        if ((state == DOWN || state == PRESSED) && (distance(startX, startY, curX, curY) > 2)) {
            if (selectedEntity && point_in_body(*selectedEntity, startX, startY)) {
                state = DRAGGING_ENTITY;
                relX = startX - selectedEntity->x;
                relY = startY - selectedEntity->y;
            } else {
                state = DRAGGING;
            }
            state = selectedEntity && point_in_body(*selectedEntity, startX, startY) ? DRAGGING_ENTITY : DRAGGING;
        }
    } else if (event->type == SDL_MOUSEBUTTONUP) {
        state = RELEASED;
        curX = event->motion.x;
        curY = event->motion.y;
    }

    if (input.one) { *drawBackground = !*drawBackground; }
    if (input.two) { *drawMidground = !*drawMidground; }
    if (input.three) { *drawForeground = !*drawForeground; }
    if (input.w) { wallTool = !wallTool; }

    if (state == PRESSED) {
        std::vector<Body>* bodies = &s->bodies;
        for (unsigned int i = 0; i < bodies->size(); i++) {
            Body& b = (*bodies)[i];
            
            if (point_in_body(b, startX, startY)) {
                selectedEntity = selectedEntity == &b ? NULL : &b;
            }
        }
    }

    if (state == RELEASED) {
        if (wallTool && (distance(startX, startY, curX, curY) > 2)) {
            int x1 = startX, y1 = startY;
            int x2 = curX, y2 = curY;
            float x = x1 <= x2 ? x1 : x2;
            float y = y1 <= y2 ? y1 : y2;
            int xp = x1 > x2 ? x1 : x2;
            int yp = y1 > y2 ? y1 : y2;
            unsigned short w = xp - x;
            unsigned short h = yp - y;
            Body b = { x, y, w, h };

            s->solidEntities.insert(s->bodies.size());
            s->bodies.push_back(b);
        }
    }

    if (state == DRAGGING_ENTITY) {
        selectedEntity->x = curX - relX;
        selectedEntity->y = curY - relY;
    }
}

void editor_draw(GraphicsContext* g) {
    g->drawGridOverlay();

    // Dividing then multiplying works, because there is a loss of info
    // when the int is floored.
    // This may be useful for tiles, but I'm not sure
    //g->drawBox((input.x / 32) * 32, (input.y / 32) * 32, 31, 31, Color::BLUE, 100);
    if (state == DRAGGING) {
        g->drawSelection(startX, startY, curX, curY);
    }

    if (selectedEntity) {
        g->drawBox(selectedEntity->x, selectedEntity->y, selectedEntity->w, selectedEntity->h, Color::BLUE, 100);
    }

    if (wallTool) {
        g->drawText(0, 0, 128, 32, "WALL TOOL");
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
