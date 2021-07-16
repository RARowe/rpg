#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "enums.h"
#include "types.h"
#include "graphics_context.h"

typedef struct {
    bool one;
    bool two;
    bool three;
    bool w;
    bool click;
    int x, y;
    bool drag;
    int startX, startY;
} EditorInput;

EditorInput input;
Body* selectedEntity = NULL;
bool wallTool = false;

void editor_handle_input(SDL_Event* event, bool* drawBackground, bool* drawMidground, bool* drawForeground, SceneData* s) {
    bool drag = input.drag;
    int x = input.x;
    int y = input.y;
    int startX = input.startX;
    int startY = input.startY;
    // Set all flags to false
    memset(&input, 0, sizeof(EditorInput));
    input.drag = drag;
    input.startX = startX;
    input.startY = startY;
    input.x = x;
    input.y = y;

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
            default:
                break;
        }
    }

    if (event->type == SDL_MOUSEBUTTONDOWN) {
        input.drag = true;
        input.startX = event->motion.x;
        input.startY = event->motion.y;
    } else if (event->type == SDL_MOUSEMOTION) {
        input.x = event->motion.x;
        input.y = event->motion.y;
    } else if (event->type == SDL_MOUSEBUTTONUP) {
        input.drag = false;
        input.click = true;
        input.x = event->motion.x;
        input.y = event->motion.y;
    }

    if (input.one) { *drawBackground = !*drawBackground; }
    if (input.two) { *drawMidground = !*drawMidground; }
    if (input.three) { *drawForeground = !*drawForeground; }
    if (input.w) { wallTool = !wallTool; }
    if (input.click) {
        // TODO: Remove this
        std::vector<Body>* bodies = &s->bodies;
        for (unsigned int i = 0; i < bodies->size(); i++) {
            Body& b = (*bodies)[i];
            
            if (point_in_body(b, input.x, input.y)) {
                selectedEntity = selectedEntity == &b ? NULL : &b;
                //printf("found entity: x,%f,y,%f,w,%d,h,%d\n", b.x, b.y, b.w, b.h);
            }
        }

        if (wallTool) {
            int x1 = input.startX, y1 = input.startY;
            int x2 = input.x, y2 = input.y;
            float x = x1 <= x2 ? x1 : x2;
            float y = y1 <= y2 ? y1 : y2;
            int xp = x1 > x2 ? x1 : x2;
            int yp = y1 > y2 ? y1 : y2;
            unsigned short w = xp - x;
            unsigned short h = yp - y;
            Body b = { x, y, w, h };

            s->solidEntities.insert(bodies->size());
            bodies->push_back(b);
            puts("adding wall");
        }
    }
}

void editor_draw(GraphicsContext* g) {
    g->drawGridOverlay();

    // Dividing then multiplying works, because there is a loss of info
    // when the int is floored.
    // This may be useful for tiles, but I'm not sure
    //g->drawBox((input.x / 32) * 32, (input.y / 32) * 32, 31, 31, Color::BLUE, 100);
    if (input.drag) {
        g->drawSelection(input.startX, input.startY, input.x, input.y);
    }

    if (selectedEntity) {
        g->drawBox(selectedEntity->x, selectedEntity->y, selectedEntity->w, selectedEntity->h, Color::BLUE, 100);
    }

    if (wallTool) {
        g->drawText(0, 0, 128, 32, "WALL TOOL");
    }
}
