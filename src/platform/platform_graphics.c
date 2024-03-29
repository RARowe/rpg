#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "platform.h"

/* SDL Graphics Platform Implementation */
typedef struct {
    SDL_Texture *texture;
    unsigned int w, h;
} Texture;

typedef struct {
    int size;
    Texture textures[32];
} TextureCache;

typedef struct Graphics {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    TextureCache textureCache;
} Graphics;

/* Internal methods */
static void color_get(Color c, uint8_t *r, uint8_t *g, uint8_t *b) {
    switch (c) {
    case COLOR_WHITE:
        *r = 255;
        *g = 255;
        *b = 255;
        break;
    case COLOR_BLUE:
        *r = 48;
        *g = 72;
        *b = 203;
        break;
    case COLOR_RED:
        *r = 255;
        *g = 0;
        *b = 0;
        break;
    case COLOR_BLACK:
    default:
        *r = 0;
        *g = 0;
        *b = 0;
        break;
    }
}

static inline Texture *graphics_get_texture(Graphics *g, int id) {
    return &g->textureCache.textures[id];
}

static SDL_Texture *font_get_texture(Graphics *gr, const char *text) {
    uint8_t r, g, b;
    color_get(COLOR_WHITE, &r, &g, &b);

    SDL_Color color = {r, g, b, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(gr->font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(gr->renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

void graphics_draw_text(Graphics *g, int x, int y, int fontSize,
                        const char *text) {
    const int charWidth = fontSize * 0.6f;
    long textLength = strlen(text);

    SDL_Rect out = {x, y, charWidth * (int)textLength, fontSize};
    SDL_Texture *texture = font_get_texture(g, text);

    SDL_RenderCopy(g->renderer, texture, NULL, &out);
    SDL_DestroyTexture(texture);
}

void graphics_draw_wrapped_text(Graphics *g, int x, int y, int fontSize,
                                int maxWidth, const char *text) {
    const int charWidth = fontSize * 0.6f;
    const int numberOfCharsPerLine = maxWidth / charWidth;

    int textLineNumber = 0;
    int numberOfCharsToTake = 1;
    int newStart = 0;
    for (int i = 0; i < strlen(text); i++) {
        if (numberOfCharsToTake == numberOfCharsPerLine) {
            int oldValueInCaseOfWordWithNoBreaks = i;
            while (text[i] != ' ') {
                i--;
                numberOfCharsToTake--;

                if (i < newStart) {
                    i = oldValueInCaseOfWordWithNoBreaks;
                    numberOfCharsToTake = numberOfCharsPerLine;
                    break;
                }
            }

            graphics_draw_text(g, x, y + (32 * textLineNumber), fontSize, text);

            textLineNumber++;
            numberOfCharsToTake = 1;
            newStart = i + 1;
        } else {
            numberOfCharsToTake++;
        }
    }

    graphics_draw_text(g, x, y + (32 * textLineNumber), fontSize, text);
}

void graphics_draw_menu(Graphics *g, int x, int y, int fontSize, char **options,
                        int n) {
    int maxStringSize = 0;
    int curStringSize = 0;

    for (int i = 0; i < n; i++) {
        while (options[i][curStringSize]) {
            curStringSize++;
        }
        curStringSize--;

        if (maxStringSize < curStringSize) {
            maxStringSize = curStringSize;
        }
        //
        curStringSize = 0;
    }

    int menuWidth = fontSize * 0.7f * maxStringSize;
    int menuHeight = (fontSize * 1.2f) * n;

    graphics_draw_box(g, x, y, menuWidth, menuHeight, COLOR_BLUE, 255);

    for (int i = 0; i < n; i++) {
        graphics_draw_text(g, x, y, fontSize, options[i]);
        y += fontSize + 2;
    }
}

void graphics_draw_texture(Graphics *g, int id, int x, int y, int w, int h) {
    Texture *t = graphics_get_texture(g, id);

    SDL_Rect in = {0, 0, (int)t->w, (int)t->h};
    SDL_Rect out = {x, y, w, h};

    SDL_RenderCopy(g->renderer, t->texture, &in, &out);
}

void graphics_draw_tiles(Graphics *g, int id, const int *tiles, int count) {
    const int width = 16;
    const int height = 16;
    const int columns = 37;
    SDL_Texture *tileSetTexture = graphics_get_texture(g, id)->texture;
    int row = 0;
    int column = 0;
    SDL_Rect in = {0, 0, width, height};
    SDL_Rect out = {0, 0, 32, 32};
    const int pixelXOffset = 17;
    const int pixelYOffset = 17;
    int p;
    for (int i = 0; i < count; i++) {
        p = tiles[i];
        in.x = (p % columns) * pixelXOffset;
        in.y = (p / columns) * pixelYOffset;
        out.x = column * 32;
        out.y = row * 32;

        SDL_RenderCopy(g->renderer, tileSetTexture, &in, &out);

        column++;
        // Hardcoded. Fix this.
        if (column == 19) {
            column = 0;
            row++;
        }
    }
}

void graphics_draw_tile(Graphics *g, int id, int tile, int x, int y, int w,
                        int h) {
    const int columns = 37;
    const int pixelXOffset = 17;
    const int pixelYOffset = 17;
    const int width = 16;
    const int height = 16;
    SDL_Texture *texture = graphics_get_texture(g, id)->texture;
    SDL_Rect in = {(tile % columns) * pixelXOffset,
                   (tile / columns) * pixelYOffset, width, height};
    SDL_Rect out = {x, y, w, h};

    SDL_RenderCopy(g->renderer, texture, &in, &out);
}

void graphics_draw_box(Graphics *gr, int x, int y, int w, int h, Color c,
                       int alpha) {
    uint8_t r, g, b;
    color_get(c, &r, &g, &b);

    SDL_SetRenderDrawBlendMode(gr->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gr->renderer, r, g, b, alpha);
    SDL_Rect rectangle = {x, y, w, h};
    SDL_RenderFillRect(gr->renderer, &rectangle);
    SDL_SetRenderDrawBlendMode(gr->renderer, SDL_BLENDMODE_NONE);
}

void graphics_draw_selection(Graphics *gr, int x1, int y1, int x2, int y2) {
    uint8_t r, g, b;
    color_get(COLOR_BLUE, &r, &g, &b);

    SDL_SetRenderDrawColor(gr->renderer, r, g, b, 255);
    SDL_RenderDrawLine(gr->renderer, x1, y1, x2, y1);
    SDL_RenderDrawLine(gr->renderer, x1, y1, x1, y2);
    SDL_RenderDrawLine(gr->renderer, x2, y2, x2, y1);
    SDL_RenderDrawLine(gr->renderer, x2, y2, x1, y2);

    int x = x1 <= x2 ? x1 : x2;
    int y = y1 <= y2 ? y1 : y2;
    int xp = x1 > x2 ? x1 : x2;
    int yp = y1 > y2 ? y1 : y2;
    int w = xp - x;
    int h = yp - y;

    graphics_draw_box(gr, SCREEN_WIDTH, y, w, h, COLOR_BLUE, 100);
}

void graphics_draw_grid_overlay(Graphics *g) {
    SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    for (int i = 31; i < SCREEN_WIDTH; i += 32) {
        SDL_RenderDrawLine(g->renderer, i, 0, i, SCREEN_HEIGHT);
    }

    for (int i = 31; i < SCREEN_HEIGHT; i += 32) {
        SDL_RenderDrawLine(g->renderer, 0, i, SCREEN_WIDTH, i);
    }
}
