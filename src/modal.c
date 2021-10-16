#include <SDL2/SDL.h>
#include "input.c"
#include "graphics_context.h"
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
        m->currentSelection = 0;
    } else if (m->currentSelection >= m->numberOfOptions) {
        m->currentSelection = m->numberOfOptions - 1;
    }

    return 0;
}

void modal_draw(GraphicsContext* g, const Modal* m, float timeStep) {
    g->drawBox(31, 31, g->width - 64, g->height - 64, Color::BLUE, 255);
    g->drawText(64, 64 + (32 * m->currentSelection), 32, ">");

    for (int i = 0; i < m->numberOfOptions; i++) {
        g->drawText(96, 64 + (32 * i), 32, m->options[i]);
    }
}

