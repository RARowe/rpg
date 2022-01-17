#include "game.h"

static int modal_handle_input(const Input* i, Modal* m) {
    if (input_is_pressed(i, GAME_INPUT_DOWN)) {
        m->currentSelection++;
    } else if (input_is_pressed(i, GAME_INPUT_UP)) {
        m->currentSelection--;
    } else if (input_is_pressed(i, GAME_INPUT_SELECT)) {
        *(m->result) = m->currentSelection;
        return 1;
    } else if (input_is_pressed(i, GAME_INPUT_BACK)) {
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

static void modal_draw(Graphics* g, const Modal* m, float timeStep) {
    graphics_draw_box(g, m->dim.x, m->dim.y, m->dim.w, m->dim.h, COLOR_BLUE, 255);

    const Point* p = &m->textStartingPoint;
    graphics_draw_text(g, p->x - 32, p->y + (32 * m->currentSelection), 32, ">");

    for (int i = 0; i < m->numberOfOptions; i++) {
        graphics_draw_text(g, p->x, p->y + (32 * i), 32, m->options[i]);
    }

    if (m->hasTitle) {
        graphics_draw_text(g, 128, 32, 64, m->title);
    }
}

static int tile_picker_handle_input(const Input* i, TilePicker* p) {
    if (input_is_pressed(i, GAME_INPUT_UP)) {
        p->tile -= p->tilesetMeta.hTiles;
    } else if (input_is_pressed(i, GAME_INPUT_DOWN)) {
        p->tile += p->tilesetMeta.hTiles;
    } else if (input_is_pressed(i, GAME_INPUT_LEFT)) {
        p->tile--;
    } else if (input_is_pressed(i, GAME_INPUT_RIGHT)) {
        p->tile++;
    }

    if (p->tile < 0) {
        p->tile = 0;
    } else if (p->tile > p->tilesetMeta.totalTiles) {
        p->tile = p->tilesetMeta.totalTiles - 1;
    }

    if (input_is_pressed(i, KEY_f)) {
        return 1;
    } else if (input_is_pressed(i, KEY_d)) {
        p->tile = -1;
        return 1;
    }

    return 0;
}

static int text_editor_handle_input(TextEditor* t, const Input* i) {
    if (i->hasLastPressedKey) {
        char ch = i->lastPressedKey;
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

static void tile_picker_draw(Graphics* g, const TilePicker* p) {
    //int textureId = p->tilesetMeta.id;
    //int tile = p->tile;
    //int numberOfHorizontalTiles = p->tilesetMeta.hTiles;

    //int col = tile % numberOfHorizontalTiles;
    //int row = tile / numberOfHorizontalTiles;

    //int maxFrameTilesHorizontal = ((width - 32) / 34) + 1;
    //int maxFrameTilesVertical = ((height - 32) / 34) + 1;
    //int frameXOffset = col - (maxFrameTilesHorizontal - 1) < 0 ?
    //    0 :
    //    col - (maxFrameTilesHorizontal - 1);
    //int frameYOffset = row - (maxFrameTilesVertical - 1) < 0 ?
    //    0 :
    //    row - (maxFrameTilesVertical - 1);

    //Texture& t = textureCache[textureId];
    //SDL_Rect in = {
    //    17 * frameXOffset,
    //    17 * frameYOffset,
    //    maxFrameTilesHorizontal * 17,
    //    maxFrameTilesVertical * 17
    //};
    //SDL_Rect out = { 0, 0, width - 1, height - 1 };
    //SDL_RenderCopy(_renderer, t.texture, &in, &out);

    //float w = ((float)width / (float)maxFrameTilesHorizontal);
    //float h = ((float)height / (float)maxFrameTilesVertical);
    //int x = (int) ((float)(maxFrameTilesHorizontal <= col ? maxFrameTilesHorizontal - 1: col) * w);
    //int y = (int) ((float)(maxFrameTilesVertical <= row ? maxFrameTilesVertical - 1 : row) * h);
    //drawBox(x, y, (int)w, (int)h, Color::WHITE, 100);
}

