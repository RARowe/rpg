#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include "graphics_context.h"
#include "inventory.h"

const int GraphicsContext::FRAME_RATE = 60;

char buffer[256];
char tBuffer[256];

static SDL_Texture* getTextureFromSurface (
    SDL_Renderer* renderer,
    SDL_Surface* surface
) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}
#define MAKEUI(a,b,c,d) ((unsigned int) ( ((unsigned int)(a)) << 24 | ((unsigned int)(b)) << 16 | ((unsigned int)(c)) << 8 | ((unsigned int)(d)) ))
void get_png_size(const char* path, unsigned int* w, unsigned int* h) {
        unsigned char b[4];
        FILE* f = fopen(path, "r");
        fseek(f, 16, SEEK_SET);

        fread(b, sizeof(unsigned char), 4, f);
        *w = MAKEUI(b[0], b[1], b[2], b[3]);
        fread(b, sizeof(unsigned char), 4, f);
        *h = MAKEUI(b[0], b[1], b[2], b[3]);

        fclose(f);
}

// TODO: This is pretty fragile, and may not be OS portable
//       It also may not read pngs properly and may cause buffer overflow
static void load_all_textures(SDL_Renderer* renderer, const char* path, std::map<unsigned int, Texture>& textureCache) {
    Texture t;
    unsigned int textureId = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    while ((dir = readdir(d)) != NULL) {
        char* name = dir->d_name;
        size_t length = strlen(name);
        if (name[length - 1] == 'g') {
            strcpy(tBuffer, path);
            strcat(tBuffer, "/");
            strcat(tBuffer, dir->d_name);
            SDL_Surface* surface = IMG_Load(tBuffer);
            t.texture = getTextureFromSurface(renderer, surface);
            strcpy(t.name, dir->d_name);
            get_png_size(tBuffer, &t.w, &t.h);

            textureCache[textureId] = t;
            textureId += 1;
        }
    }
    closedir(d);
}

GraphicsContext::GraphicsContext(const char* title, int w, int h, const char* resourceFolderPath)
{
    width = w;
    height = h;

    if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO) < 0)
    {
        std::cout << "SDL could not be initialized! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    _window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
    );
    if (_window == NULL) {
        std::cout << "Could not create window! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (_renderer == NULL) {
        std::cout << "Could not create renderer! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() < 0)
    {
        std::cout << "SDL TTF could not be initialized! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    char buffer[256];
    strcpy(buffer, resourceFolderPath);
    strcat(buffer, "slkscr.ttf");
    _font = TTF_OpenFont(buffer, 16);
    _resourceFolderPath = resourceFolderPath;
    load_all_textures(_renderer, "resources/tile_sets", textureCache);
}

GraphicsContext::~GraphicsContext()
{
    TTF_CloseFont(_font);
    TTF_Quit();
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    for (auto& keyPair : textureCache)
    {
        SDL_DestroyTexture(keyPair.second.texture);
    }
    SDL_Quit();
}

void GraphicsContext::drawText(int x, int y, int w, int h, const char* text)
{
    SDL_Rect out = {x, y, w, h};
    SDL_Texture* texture = getFontTexture(text);
    SDL_RenderCopy(_renderer, texture, NULL, &out);
    SDL_DestroyTexture(texture);
}

void GraphicsContext::drawText(int x, int y, int fontSize, const std::string& text)
{
    const char* cText = text.c_str();
    const int charWidth = fontSize * 0.6f;
    SDL_Rect out = { x, y, charWidth * (int)text.size(), fontSize };
    SDL_Texture* texture = getFontTexture(cText);
    SDL_RenderCopy(_renderer, texture, NULL, &out);
    SDL_DestroyTexture(texture);
}

void GraphicsContext::drawWrappedText(int x, int y, int fontSize, int maxWidth, const std::string& text)
{
    const int charWidth = fontSize * 0.6f;
    const int numberOfCharsPerLine = maxWidth / charWidth;

    int textLineNumber = 0;
    int numberOfCharsToTake = 1;
    int newStart = 0;
    for (int i = 0; i < text.size(); i++)
    {
        if (numberOfCharsToTake == numberOfCharsPerLine)
        {
            int oldValueInCaseOfWordWithNoBreaks = i;
            while (text[i] != ' ')
            {
                i--;
                numberOfCharsToTake--;

                if (i < newStart)
                {
                    i = oldValueInCaseOfWordWithNoBreaks;
                    numberOfCharsToTake = numberOfCharsPerLine;
                    break;
                }
            }
            const std::string& lineText = text.substr(newStart, numberOfCharsToTake);
            drawText(x, y + (32 * textLineNumber), fontSize, lineText);

            textLineNumber++;
            numberOfCharsToTake = 1;
            newStart = i + 1;
        }
        else
        {
            numberOfCharsToTake++;
        }
    }
    const std::string& lineText = text.substr(newStart, numberOfCharsToTake);
    drawText(x, y + (32 * textLineNumber), fontSize, lineText);
}

void GraphicsContext::drawTiles(unsigned int id, const int* tiles, size_t count)
{
    const int width = 16;
    const int height = 16;
    const int columns = 37;
    SDL_Texture* tileSetTexture = textureCache[id].texture;
    int row = 0;
    int column = 0;
    SDL_Rect in = { 0, 0, width, height };
    SDL_Rect out = { 0, 0, 32, 32 };
    const int pixelXOffset = 17;
    const int pixelYOffset = 17;
    int p;
    for (unsigned int i = 0; i < count; i++) {
        p = tiles[i];
        in.x = (p % columns) * pixelXOffset;
        in.y = (p / columns) * pixelYOffset;
        out.x = column * 32;
        out.y = row * 32;

        SDL_RenderCopy(_renderer, tileSetTexture, &in, &out);

        column++;
        // Hardcoded. Fix this.
        if (column == 19)
        {
            column = 0;
            row++;
        }
    }
}

void GraphicsContext::drawTile(unsigned int id, int tile, int x, int y, int w, int h)
{
    const int columns = 37;
    const int pixelXOffset = 17;
    const int pixelYOffset = 17;
    const int width = 16;
    const int height = 16;
    SDL_Texture* texture = textureCache[id].texture;
    SDL_Rect in =
    {
        (tile % columns) * pixelXOffset,
        (tile / columns) * pixelYOffset, 
        width,
        height
    };
    SDL_Rect out = { x, y, w, h };

    SDL_RenderCopy(_renderer, texture, &in, &out);
}

void GraphicsContext::drawHitbox(int x, int y, int w, int h)
{
    if (_showHitboxes)
    {
        SDL_Rect out = { x, y, w, h };
        SDL_RenderDrawRect(_renderer, &out);
    }
}

typedef struct RGBValues
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
} RGBValues;

static const RGBValues getColor(Color c)
{
    switch (c)
    {
        case Color::WHITE:
            return { 255, 255, 255 };
        case Color::BLUE:
            return { (Uint8)48, (Uint8)72, (Uint8)203 };
        case Color::RED:
            return { (Uint8)255, 0, 0 };
        case Color::BLACK:
        default:
            return { 0, 0, 0 };
    }
}

void GraphicsContext::drawBox(int x, int y, int w, int h, Color c, int alpha)
{
    auto rgb = getColor(c);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(_renderer, rgb.r, rgb.g, rgb.b, alpha);
    SDL_Rect rectangle = { x, y, w, h };
    SDL_RenderFillRect(_renderer, &rectangle);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_NONE);
}

void GraphicsContext::drawSelection(int x1, int y1, int x2, int y2) {
    auto rgb = getColor(Color::BLUE);
    SDL_SetRenderDrawColor(_renderer, rgb.r, rgb.g, rgb.b, 255);
    SDL_RenderDrawLine(_renderer, x1, y1, x2, y1);
    SDL_RenderDrawLine(_renderer, x1, y1, x1, y2);
    SDL_RenderDrawLine(_renderer, x2, y2, x2, y1);
    SDL_RenderDrawLine(_renderer, x2, y2, x1, y2);

    int x = x1 <= x2 ? x1 : x2;
    int y = y1 <= y2 ? y1 : y2;
    int xp = x1 > x2 ? x1 : x2;
    int yp = y1 > y2 ? y1 : y2;
    int w = xp - x;
    int h = yp - y;

    drawBox(x, y, w, h, Color::BLUE, 100);
}

void GraphicsContext::drawOnGridAt
(
    int x,
    int y,
    int cellWidth,
    int cellHeight,
    int cellMargin,
    int row,
    int column,
    std::function<void (int, int, int, int)> drawFunction
)
{
    int xIncrementValue = x + cellMargin;
    int yIncrementValue = y + cellMargin;
    int newX = xIncrementValue;
    int newY = yIncrementValue;

    drawFunction(newX * column, newY * row, cellWidth, cellHeight);
}

void GraphicsContext::present()
{
    SDL_RenderPresent(_renderer);
}

void GraphicsContext::toggleHitboxView()
{
    _showHitboxes = !_showHitboxes;
}

WindowPosition GraphicsContext::getPosition(int x, int y) const
{
    if (x > width)
    {
        return WindowPosition::RIGHT;
    }
    else if (x < 0)
    {
        return WindowPosition::LEFT;
    }
    else if (y > height)
    {
        return WindowPosition::BELOW;
    }
    else if (y < 0)
    {
        return WindowPosition::ABOVE;
    }
    else
    {
        return WindowPosition::INSIDE;
    }
}

SDL_Texture* GraphicsContext::getFontTexture(const std::string& text)
{
    return getFontTexture(text.c_str());
}

SDL_Texture* GraphicsContext::getFontTexture(const char* text)
{
    SDL_Color color = { 255, 255, 255 };
    SDL_Surface * surface = TTF_RenderText_Solid(_font, text, color);

    return getTextureFromSurface(_renderer, surface);
}

void GraphicsContext::drawGridOverlay() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    unsigned int i;
    for (i = 31; i < width; i += 32) {
        SDL_RenderDrawLine(_renderer, i, 0, i, height);
    }

    for (i = 31; i < height; i += 32) {
        SDL_RenderDrawLine(_renderer, 0, i, width, i);
    }
}

// TODO: This is duplicate of draw grid. These will need to be rethought
void GraphicsContext::drawInventory
(
    int x,
    int y,
    int numberOfRows,
    int numberOfColumns,
    int cellWidth,
    int cellHeight,
    int cellMargin,
    Inventory* inventory
)
{
    //ItemType* items = inventory->items;
    int xIncrementValue = x + cellMargin;
    int yIncrementValue = y + cellMargin;
    int newX = xIncrementValue;
    int newY = yIncrementValue;
    int row = 1;
    int column = 1;

    for (int8_t i = 0; i < inventory->size; i++)
    {
        //drawTile(TileSets::ITEMS, (int)inventory_get_item_texture(items[i]), newX, newY, cellWidth, cellHeight);
        column++;
        if (column > numberOfColumns)
        {
            column = 1;
            row++;

            if (row > numberOfRows)
            {
                break;
            }
        }

        newX = column * xIncrementValue;
        newY = row * yIncrementValue;
    }
}
