#include <SDL2/SDL_image.h>
#include <iostream>
#include <string.h>
#include "graphics_context.h"

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
