#include <SDL2/SDL.h>
#include "input.c"
#include "graphics.h"
#include "types.h"

int modal_handle_input(const Input* i, Modal* m) {
    if (input_is_pressed(i, SDLK_DOWN)) {
        m->currentSelection++;
    } else if (input_is_pressed(i, SDLK_UP)) {
        m->currentSelection--;
    } else if (input_is_pressed(i, SDLK_f)) {
        *(m->result) = m->currentSelection;
        return 1;
    } else if (input_is_pressed(i, SDLK_d)) {
        *(m->result) = -1;
        return 1;
    }

    /* Correct currentSelection if out of range */
    if (m->currentSelection < 0) {
        m->currentSelection = m->numberOfOptions - 1;
    } else if (m->currentSelection >= m->numberOfOptions) {
        m->currentSelection = 0;
    }

    return 0;
}

void modal_draw(Graphics* g, const Modal* m, float timeStep) {
    g->drawBox(m->dim.x, m->dim.y, m->dim.w, m->dim.h, Color::BLUE, 255);

    const Point* p = &m->textStartingPoint;
    g->drawText(p->x - 32, p->y + (32 * m->currentSelection), 32, ">");

    for (int i = 0; i < m->numberOfOptions; i++) {
        g->drawText(p->x, p->y + (32 * i), 32, m->options[i]);
    }

    if (m->hasTitle) {
        g->drawText(128, 32, 64, m->title);
    }
}

int tile_picker_handle_input(const Input* i, TilePicker* p) {
    if (input_is_pressed(i, SDLK_UP)) {
        p->tile -= p->tilesetMeta.hTiles;
    } else if (input_is_pressed(i, SDLK_DOWN)) {
        p->tile += p->tilesetMeta.hTiles;
    } else if (input_is_pressed(i, SDLK_LEFT)) {
        p->tile--;
    } else if (input_is_pressed(i, SDLK_RIGHT)) {
        p->tile++;
    }

    if (p->tile < 0) {
        p->tile = 0;
    } else if (p->tile > p->tilesetMeta.totalTiles) {
        p->tile = p->tilesetMeta.totalTiles - 1;
    }

    if (input_is_pressed(i, SDLK_f)) {
        return 1;
    } else if (input_is_pressed(i, SDLK_d)) {
        p->tile = -1;
        return 1;
    }

    return 0;
}

int text_editor_handle_input(TextEditor* t, const Input* i) {
    char ch;
    if (input_get_last_pressed_key(i, &ch)) {
        if (ch == '\r') {
            strcpy(t->outBuffer, t->buffer);
            bzero(t->buffer, sizeof(char) * 1024);
            t->cursorPos = 0;

            return 1;
        } else if (ch == '\b') {
            t->buffer[t->cursorPos] = '\0';
            t->cursorPos -= 1;
            t->cursorPos = t->cursorPos < 0 ? 0 : t->cursorPos;

            return 0;
        } else {
            t->buffer[t->cursorPos] = ch;
            t->cursorPos += 1;

            return 0;
        }
    }

    return 0;
}

/* Modal selector index */
//int cursorIndex = 0;
//static void editor_handle_input_modal(GameContext* c, Graphics* g, Input* i, SceneData* s) {
//    if (input_is_pressed(i, SDLK_UP)) { 
//        cursorIndex = cursorIndex - 1 < 0 ? 0 : cursorIndex - 1;
//    }
//
//    if (input_is_pressed(i, SDLK_DOWN)) {
//        cursorIndex = cursorIndex + 1 == g->textureCache.size() ?
//            cursorIndex :
//            cursorIndex + 1;
//    }
//
//    if (input_is_pressed(i, SDLK_f) && openTexture) {
//        s->tileSet = cursorIndex;
//        openTexture = false;
//        requestedNextEditorMode = EDITOR_MODE_OBJECT;
//    }
//
//    if (input_is_pressed(i, SDLK_d)) {
//        openTexture = false;
//        requestedNextEditorMode = EDITOR_MODE_OBJECT;
//    }
//}
    //if (editorMode == EDITOR_MODE_MODAL_OPEN && openTexture) {
    //    g->drawBox(31, 31, g->width - 64, g->height - 64, Color::BLUE, 255);
    //    g->drawText(64, 64 + (32 * cursorIndex), 32, ">");

    //    int i = 0;
    //    for (auto&& t : g->textureCache) {
    //        g->drawText(96, 64 + i, 32, t.second.name);
    //        i += 32;
    //    }
    //}
