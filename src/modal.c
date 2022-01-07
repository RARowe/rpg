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

