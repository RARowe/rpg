#include <SDL2/SDL_image.h>
#include <iostream>
#include <string.h>
#include "graphics_context.h"
#include "tile_set.h"

const int GraphicsContext::FRAME_RATE = 60;

GraphicsContext::GraphicsContext(const char* title, int width, int height, const char* resourceFolderPath)
{
    _width = width;
    _height = height;

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
}

GraphicsContext::~GraphicsContext()
{
    TTF_Quit();
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    TTF_CloseFont(_font);
    SDL_DestroyTexture(_emoteSheet);
    for (auto& keyPair : _textureCache)
    {
        SDL_DestroyTexture(keyPair.second);
    }
    SDL_Quit();
}

SDL_Window* GraphicsContext::getWindow()
{
    return _window;
}

SDL_Renderer* GraphicsContext::getRenderer()
{
    return _renderer;
}

static SDL_Texture* getTextureFromSurface
(
    SDL_Renderer* renderer,
    SDL_Surface* surface
)
{
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

SDL_Texture* GraphicsContext::getTexture(const char* path) const
{
    char buffer[256];
    strcpy(buffer, _resourceFolderPath);
    strcat(buffer, path);
    SDL_Surface* surface = IMG_Load(buffer);

    return getTextureFromSurface(_renderer, surface);
}

void GraphicsContext::drawTexture(const Entity& e, const std::string& name)
{
    if (_textureCache.count(name) == 0)
    {
        _textureCache[name] = getTexture(name.c_str());
    }

    SDL_Rect out = { (int)e.getX(), (int)e.getY(), e.getW(), e.getH() };
    SDL_RenderCopy(_renderer, _textureCache[name], NULL, &out);

    if (_showHitboxes)
    {
        SDL_RenderDrawRect(_renderer, &out);
    }
}

void GraphicsContext::drawTexture(int x, int y, int w, int h, const std::string& name)
{
    if (_textureCache.count(name) == 0)
    {
        _textureCache[name] = getTexture(name.c_str());
    }

    SDL_Rect out = { x, y, w, h };
    SDL_RenderCopy(_renderer, _textureCache[name], NULL, &out);
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

void GraphicsContext::drawEmote(const Entity& e, const std::string& name)
{
    if (_emoteSheet == nullptr) { _emoteSheet = getTexture("emote.png"); }
    SDL_Rect in = { 64, 48, 16, 16 };
    SDL_Rect out = { (int)e.getX() + (e.getW() / 5), (int)e.getY() - 20, 16, 16 };

    SDL_RenderCopy(_renderer, _emoteSheet, &in, &out);
}

void GraphicsContext::drawAbove(const Entity& e, TileSets tileSet, int tile)
{
    drawTile(tileSet, tile, (int)e.getX() + (e.getW() / 4), (int)e.getY() - 36, 16, 16);
}

void GraphicsContext::drawTiles(TileSets tileSet, const std::vector<int>& positions)
{
    const TileSetData& tileSetData = getTileSetData(tileSet);
    const std::string& name = tileSetData.path;
    const int width = tileSetData.tileWidth;
    const int height = tileSetData.tileHeight;

    if (_textureCache.count(name) == 0)
    {
        _textureCache[name] = getTexture(name.c_str());
    }
    SDL_Texture* tileSetTexture = _textureCache[name];
    int row = 0;
    int column = 0;
    SDL_Rect in = { 0, 0, width, height };
    SDL_Rect out = { 0, 0, 32, 32 };
    const int pixelXOffset = width + tileSetData.margin;
    const int pixelYOffset = height + tileSetData.margin;
    for (auto p : positions)
    {
        in.x = (p % tileSetData.columns) * pixelXOffset;
        in.y = (p / tileSetData.columns) * pixelYOffset;
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

void GraphicsContext::drawTile(TileSets tileSet, int tile, int x, int y, int w, int h)
{
    const TileSetData& tileSetData = getTileSetData(tileSet);
    const std::string& tileSetName = tileSetData.path;
    const int columns = tileSetData.columns;
    const int pixelXOffset = tileSetData.tileWidth + tileSetData.margin;
    const int pixelYOffset = tileSetData.tileHeight + tileSetData.margin;
    if (_textureCache.count(tileSetName) == 0)
    {
        _textureCache[tileSetName] = getTexture(tileSetName.c_str());
    }
    SDL_Rect in =
    {
        (tile % columns) * pixelXOffset,
        (tile / columns) * pixelYOffset, 
        tileSetData.tileWidth,
        tileSetData.tileHeight
    };
    SDL_Rect out = { x, y, w, h };

    SDL_RenderCopy(_renderer, _textureCache[tileSetName], &in, &out);
}

void GraphicsContext::drawHitbox(int x, int y, int w, int h)
{
    if (_showHitboxes)
    {
        SDL_Rect out = { x, y, w, h };
        SDL_RenderDrawRect(_renderer, &out);
    }
}

void GraphicsContext::drawBox(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b)
{
    SDL_SetRenderDrawColor(_renderer, r, g, b, 255);
    SDL_Rect rectangle = { x, y, w, h };
    SDL_RenderFillRect(_renderer, &rectangle);
}

void GraphicsContext::present()
{
    SDL_RenderPresent(_renderer);
}

void GraphicsContext::toggleHitboxView()
{
    _showHitboxes = !_showHitboxes;
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

int GraphicsContext::getWidth()
{
    return _width;
}

int GraphicsContext::getHeight()
{
    return _height;
}
