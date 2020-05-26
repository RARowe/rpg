#include <SDL2/SDL_image.h>
#include <fstream>
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

SDL_Texture* GraphicsContext::getTexture(const std::string& path)
{
    if (_textureCache.count(path) == 0)
    {
        char buffer[256];
        strcpy(buffer, _resourceFolderPath);
        strcat(buffer, path.c_str());
        SDL_Surface* surface = IMG_Load(buffer);
        SDL_Texture* texture = getTextureFromSurface(_renderer, surface);
        _textureCache[path] = texture;
        return texture;
    }
    else
    {
        return _textureCache[path];
    }
}

void GraphicsContext::drawTexture(const Entity& e, const std::string& name)
{
    SDL_Texture* texture = getTexture(name);

    SDL_Rect out = { (int)e.getX(), (int)e.getY(), e.getW(), e.getH() };
    SDL_RenderCopy(_renderer, texture, NULL, &out);

    if (_showHitboxes)
    {
        SDL_RenderDrawRect(_renderer, &out);
    }
}

void GraphicsContext::drawTexture(int x, int y, int w, int h, const std::string& name)
{
    SDL_Texture* texture = getTexture(name);

    SDL_Rect out = { x, y, w, h };
    SDL_RenderCopy(_renderer, texture, NULL, &out);
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
    drawTile(tileSet, tile, (int)e.getX() + (e.getW() / 4), (int)e.getY() - 24, 16, 16);
}

void GraphicsContext::drawTiles(TileSets tileSet, const std::vector<int>& positions)
{
    const TileSetData& tileSetData = getTileSetData(tileSet);
    const std::string& name = tileSetData.path;
    const int width = tileSetData.tileWidth;
    const int height = tileSetData.tileHeight;
    SDL_Texture* tileSetTexture = getTexture(name);
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
    SDL_Texture* texture = getTexture(tileSetName);
    SDL_Rect in =
    {
        (tile % columns) * pixelXOffset,
        (tile / columns) * pixelYOffset, 
        tileSetData.tileWidth,
        tileSetData.tileHeight
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
        default:
            return { 0, 0, 0 };
    }
}

void GraphicsContext::drawBox(int x, int y, int w, int h, Color c)
{
    auto rgb = getColor(c);
    SDL_SetRenderDrawColor(_renderer, rgb.r, rgb.g, rgb.b, 255);
    SDL_Rect rectangle = { x, y, w, h };
    SDL_RenderFillRect(_renderer, &rectangle);
}

void GraphicsContext::drawSprite(const std::string& spriteSheet, int sprite, const Entity& e)
{
    auto&& spriteData = getSpriteData(spriteSheet);
    int spriteX = (sprite % spriteData.columns) * spriteData.spriteWidth;
    int spriteY = (sprite / spriteData.columns) * spriteData.spriteHeight;
    SDL_Rect src = { spriteX, spriteY, spriteData.spriteWidth, spriteData.spriteHeight };
    SDL_Rect out = { (int)e.getX(), (int)e.getY(), e.getW(), e.getH() };
    SDL_RenderCopy(_renderer, spriteData.texture, &src, &out);
}

const SpriteData& GraphicsContext::getSpriteData(const std::string& spriteSheet)
{
    if (_spriteCache.count(spriteSheet) == 0)
    {
        SDL_Texture* texture = getTexture("sprite_sheets/" + spriteSheet + "/sheet.png");

        std::string line;
   	    std::ifstream infile("resources/sprite_sheets/" + spriteSheet + "/info.txt");

        char type = (char)0;
        int count = 0;
   	    if (infile) {
            std::unique_ptr<SpriteData> s = std::make_unique<SpriteData>();
            s->texture = texture;
            Uint32 _junkFormat = 0;
            int _junkAccess = 0;
            int w = 0;
            int h = 0;
            SDL_QueryTexture(texture, &_junkFormat, &_junkAccess, &w, &h);
   	        while (std::getline(infile, line))
            {
                std::sscanf(line.c_str(), "%c,%d", &type, &count);
                switch (type)
                {
                    case 'r':
                        s->rows = count;
                        s->spriteHeight = h / count;
                        break;
                    case 'c':
                    default:
                        s->columns = count;
                        s->spriteWidth = w / count;
                        break;
                }
   	        }
            _spriteCache[spriteSheet] = std::move(s);
   	    }
   	    infile.close();
    }

    return *_spriteCache[spriteSheet];
}

void GraphicsContext::drawStandingSprite(Direction d, const std::string& spriteSheet, const Entity& e)
{
    switch (d)
    {
        case Direction::LEFT:
            drawSprite(spriteSheet, 6, e);
            break;
        case Direction::RIGHT:
            drawSprite(spriteSheet, 9, e);
            break;
        case Direction::UP:
            drawSprite(spriteSheet, 3, e);
            break;
        case Direction::DOWN:
            drawSprite(spriteSheet, 0, e);
            break;
    }
}

void GraphicsContext::drawWalkingSprite(TimeStep t, Direction d, const std::string& spriteSheet, const Entity& e)
{
    int frame = (int)((t.getTotalTime() - floor(t.getTotalTime())) * 4);
    switch (d)
    {
        case Direction::LEFT:
            if (frame == 0 || frame == 2) { drawSprite(spriteSheet, 6, e); }
            else if (frame == 1) { drawSprite(spriteSheet, 7, e); }
            else  { drawSprite(spriteSheet, 8, e); }
            break;
        case Direction::RIGHT:
            if (frame == 0 || frame == 2) { drawSprite(spriteSheet, 9, e); }
            else if (frame == 1) { drawSprite(spriteSheet, 10, e); }
            else  { drawSprite(spriteSheet, 11, e); }
            break;
        case Direction::UP:
            if (frame == 0 || frame == 2) { drawSprite(spriteSheet, 3, e); }
            else if (frame == 1) { drawSprite(spriteSheet, 4, e); }
            else  { drawSprite(spriteSheet, 5, e); }
            break;
        case Direction::DOWN:
            if (frame == 0 || frame == 2) { drawSprite(spriteSheet, 0, e); }
            else if (frame == 1) { drawSprite(spriteSheet, 1, e); }
            else  { drawSprite(spriteSheet, 2, e); }
            break;
    }
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
    if (x > _width)
    {
        return WindowPosition::RIGHT;
    }
    else if (x < 0)
    {
        return WindowPosition::LEFT;
    }
    else if (y > _height)
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