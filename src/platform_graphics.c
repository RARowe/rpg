#include "platform.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/* Get PNG dimensions */
#define MAKEUI(a,b,c,d) \
    ((unsigned int) ( \
    ((unsigned int)(a)) << 24 | \
    ((unsigned int)(b)) << 16 | \
    ((unsigned int)(c)) << 8 | \
    ((unsigned int)(d)) ))

/* Internal Declarations */
typedef struct Texture;
typedef struct TextureCache;

static SDL_Texture* font_get_texture(Graphics* g, const char* text);
static Texture* graphics_get_texture(Graphics* g, int id);
static void color_get(Color c, uint8_t* r, uint8_t* g, uint8_t* b);

/* SDL Graphics Platform Implementation */
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    const char* resourceFolderPath;
    TextureCache textureCache;
} Graphics;

void
graphics_draw_text(Graphics* g, int x, int y, int w, int h, const char* text) {
    SDL_Rect out = {x, y, w, h};
    SDL_Texture* texture = font_get_texture(g, text);
    SDL_RenderCopy(g->renderer, texture, NULL, &out);
    SDL_DestroyTexture(texture);
}

void
graphics_draw_text(Graphics* g, int x, int y, int fontSize, const char* text) {
    const int charWidth = fontSize * 0.6f;
    size_t textLength = strlen(text);

    SDL_Rect out = { x, y, charWidth * (int)textLength, fontSize };
    SDL_Texture* texture = font_get_texture(text);

    SDL_RenderCopy(g->renderer, texture, NULL, &out);
    SDL_DestroyTexture(texture);
}

void
graphics_draw_menu(Graphics* g, int x, int y, int fontSize, char** options, int n) {
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

void
graphics_draw_wrapped_text(Graphics* g, int x, int y, int fontSize, int maxWidth, const char text) {
    const int charWidth = fontSize * 0.6f;
    const int numberOfCharsPerLine = maxWidth / charWidth;

    int textLineNumber = 0;
    int numberOfCharsToTake = 1;
    int newStart = 0;
    for (int i = 0; i < text.size(); i++) {
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
            //TODO REvisit
            //const std::string& lineText = text.substr(newStart, numberOfCharsToTake);
            graphics_draw_text(g, x, y + (32 * textLineNumber), fontSize, text);

            textLineNumber++;
            numberOfCharsToTake = 1;
            newStart = i + 1;
        }
        else {
            numberOfCharsToTake++;
        }
    }
    //TODO REvisit
    //const std::string& lineText = text.substr(newStart, numberOfCharsToTake);
    graphics_draw_text(g, x, y + (32 * textLineNumber), fontSize, text);
}

void
graphics_draw_texture(Graphics* g, int id, int x, int y, int w, int h) {
    Texture* t = graphics_get_texture(g, id);

    SDL_Rect in = { 0, 0, (int)t->w, (int)t->h };
    SDL_Rect out = { x, y, w, h };

    SDL_RenderCopy(g->renderer, t->texture, &in, &out);
}

void
graphics_draw_tiles(Graphics* g, int id, const int* tiles, size_t count) {
    const int width = 16;
    const int height = 16;
    const int columns = 37;
    SDL_Texture* tileSetTexture = graphics_get_texture(g, id)->texture;
    int row = 0;
    int column = 0;
    SDL_Rect in = { 0, 0, width, height };
    SDL_Rect out = { 0, 0, 32, 32 };
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
        if (column == 19)
        {
            column = 0;
            row++;
        }
    }
}

void
graphics_draw_tile(Graphics* g, int id, int tile, int x, int y, int w, int h) {
    const int columns = 37;
    const int pixelXOffset = 17;
    const int pixelYOffset = 17;
    const int width = 16;
    const int height = 16;
    SDL_Texture* texture = graphics_get_texture(g, id)->texture;
    SDL_Rect in =
    {
        (tile % columns) * pixelXOffset,
        (tile / columns) * pixelYOffset, 
        width,
        height
    };
    SDL_Rect out = { x, y, w, h };

    SDL_RenderCopy(g->renderer, texture, &in, &out);
}

void
graphics_draw_box(Graphics* g, int x, int y, int w, int h, Color c, int alpha) {
    uint8_t r, g, b;
    color_get(c, &r, &g, &b);

    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(_renderer, r, g, b, alpha);
    SDL_Rect rectangle = { x, y, w, h };
    SDL_RenderFillRect(_renderer, &rectangle);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_NONE);
}

void
graphics_draw_selection(Graphics* g, int x1, int y1, int x2, int y2) {
    uint8_t r, g, b;
    color_get(COLOR_BLUE, &r, &g, &b);

    SDL_SetRenderDrawColor(g->renderer, r, g, b, 255);
    SDL_RenderDrawLine(g->renderer, x1, y1, x2, y1);
    SDL_RenderDrawLine(g->renderer, x1, y1, x1, y2);
    SDL_RenderDrawLine(g->renderer, x2, y2, x2, y1);
    SDL_RenderDrawLine(g->renderer, x2, y2, x1, y2);

    int x = x1 <= x2 ? x1 : x2;
    int y = y1 <= y2 ? y1 : y2;
    int xp = x1 > x2 ? x1 : x2;
    int yp = y1 > y2 ? y1 : y2;
    int w = xp - x;
    int h = yp - y;

    graphics_draw_box(g->x, y, w, h, COLOR_BLUE, 100);
}

void
graphics_draw_grid_overlay(Graphics* g) {
    SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    for (int i = 31; i < width; i += 32) {
        SDL_RenderDrawLine(g->renderer, i, 0, i, height);
    }

    for (int i = 31; i < height; i += 32) {
        SDL_RenderDrawLine(g->renderer, 0, i, width, i);
    }
}

int
graphics_get_number_of_textures(Graphics* g) {
    return g->textureCache->size;
}

/* Internal Definitions */
typedef struct {
    SDL_Texture* texture;
    unsigned int w, h;
    char name[64];
} Texture;

typedef struct {
    int size;
    Texture textures[32];
} TextureCache;

static int
graphics_init(Graphics* g, const char* title, int w, int h, const char* resourceFolderPath) {
    g->window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (g->window == NULL) {
        printf("Could not create window! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    g->renderer = SDL_CreateRenderer(
            g->window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (g->renderer == NULL) {
        printf("Could not create renderer! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    if (TTF_Init() < 0) {
        printf("SDL TTF could not be initialized! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    char buffer[256];
    strcpy(buffer, resourceFolderPath);
    strcat(buffer, "slkscr.ttf");
    g->font = TTF_OpenFont(buffer, 16);
    g->resourceFolderPath = resourceFolderPath;
    /* Load textures */
    {
        char tBuffer[256];
        Texture t;
        DIR *d;
        struct dirent *dir;
        d = opendir(path);
        while ((dir = readdir(d)) != NULL) {
            const char* name = dir->d_name;
            size_t length = strlen(name);
            if (name[length - 1] == 'g') {
                strcpy(tBuffer, path);
                strcat(tBuffer, "/");
                strcat(tBuffer, dir->d_name);

                sscanf(dir->d_name, "%d.png", &textureId);

                SDL_Surface* surface = IMG_Load(tBuffer);
                t.texture = SDL_CreateTextureFromSurface(r, surface);
                SDL_FreeSurface(surface);

                strcpy(t.name, dir->d_name);
                get_png_size(tBuffer, &t.w, &t.h);
                /* Get PNG size */
                {
                    unsigned char b[4];
                    FILE* f = fopen(path, "r");
                    fseek(f, 16, SEEK_SET);

                    fread(b, sizeof(unsigned char), 4, f);
                    t.w = MAKEUI(b[0], b[1], b[2], b[3]);
                    fread(b, sizeof(unsigned char), 4, f);
                    t.h = MAKEUI(b[0], b[1], b[2], b[3]);

                    fclose(f);
                }

                memcpy(&textureCache->textures[textureId], &t, sizeof(Texture));
                textureCache->size += 1;
            }
        }

        closedir(d);
    }

    return 1;
}

static void
graphics_shutdown(Graphics* g) {
    TTF_CloseFont(_font);
    TTF_Quit();
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    for (auto& keyPair : textureCache) {
        SDL_DestroyTexture(keyPair.second.texture);
    }
    SDL_Quit();
}

static void
graphics_present(Graphics* g) {
    SDL_RenderPresent(g->renderer);
}

static Texture*
graphics_get_texture(Graphics* g, int id) {
    return g->textureCache->textures[id];
}

static SDL_Texture*
font_get_texture(Graphics* g, const char* text) {
    uint8_t r, g, b;
    color_get(COLOR_WHITE, &r, &g, &b);

    SDL_Surface * surface = TTF_RenderText_Solid(g->font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(g->renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

static void
color_get(Color c, uint8_t* r, uint8_t* g, uint8_t* b) {
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

