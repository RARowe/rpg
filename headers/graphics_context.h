#include <SDL2/SDL.h>

#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H 
class GraphicsContext
{
    public:
        GraphicsContext(const char* title, int width, int height, const char* resourceFolderPath);
        ~GraphicsContext();
        SDL_Window* getWindow();
        SDL_Renderer* getRenderer();
        SDL_Texture* getTexture(const char* path);
        int getWidth();
        int getHeight();
        const static int FRAME_RATE;
    private:
        SDL_Window* _window;
        SDL_Renderer* _renderer;
        const char* _resourceFolderPath;
        int _width;
        int _height;
};
#endif
