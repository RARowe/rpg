#include <SDL2/SDL.h>
#include "input.c"
#include "graphics.h"
#include "types.h"


int modal_handle_input(const Input* i, Modal* m) {
    // TODO: Change this to use game controller code
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

    // Correct currentSelection if out of range
    if (m->currentSelection < 0) {
        m->currentSelection = m->numberOfOptions - 1;
    } else if (m->currentSelection >= m->numberOfOptions) {
        m->currentSelection = 0;
    }

    return 0;
}

void modal_draw(Graphics* g, const Modal* m, float timeStep) {
    g->drawBox(31, 31, g->width - 64, g->height - 64, Color::BLUE, 255);
    g->drawText(64, 64 + (32 * m->currentSelection), 32, ">");

    for (int i = 0; i < m->numberOfOptions; i++) {
        g->drawText(96, 64 + (32 * i), 32, m->options[i]);
    }
}

// TODO: This may not need to be dynamic.
//       We may be able to assume this info after load.
static inline unsigned int get_number_of_tiles(int dimension) {
    return ((dimension - 16) / 17) + 1;
}

void tile_picker_init(TilePicker* p, unsigned int id, Texture* t) {
    p->tilesetMeta.id = id;
    p->tilesetMeta.hTiles = get_number_of_tiles(t->w);
    p->tilesetMeta.vTiles = get_number_of_tiles(t->h);
    p->tilesetMeta.totalTiles = p->tilesetMeta.hTiles * p->tilesetMeta.vTiles;
    p->tile = 0;
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
        p->tile = p->tilesetMeta.totalTiles;
    }

    if (input_is_pressed(i, SDLK_f)) {
        return 1;
    } else if (input_is_pressed(i, SDLK_d)) {
        p->tile = -1;
        return 1;
    }

    return 0;
}

