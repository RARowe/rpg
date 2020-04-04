#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
class GraphicsContext
{
    public:
        GraphicsContext(const char* title, int width, int height, const char* resourceFolderPath);
        ~GraphicsContext();
        SDL_Window* getWindow();
        SDL_Renderer* getRenderer();
        SDL_Texture* getTexture(const char* path) const;
        SDL_Texture* getFontTexture(const char* text);
        SDL_Texture* getFontTexture(const std::string& text);
        int getWidth();
        int getHeight();
        const static int FRAME_RATE;
    private:
        SDL_Window* _window;
        SDL_Renderer* _renderer;
        TTF_Font* _font;
        const char* _resourceFolderPath;
        int _width;
        int _height;
};
#endif
