#ifndef GRAPHICS_H
#define GRAPHICS_H 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>
#include "types.h"

class Graphics {
    public:
        ~Graphics();
        void init(const char* title, int width, int height, const char* resourceFolderPath);
        void drawText(int x, int y, int w, int h, const char* text);
        void drawText(int x, int y, int fontSize, const std::string& text);
        void drawWrappedText(int x, int y, int fontSize, int maxWidth, const std::string& text);
        void drawTexture(unsigned int id, int x, int y, int w, int h);
        void drawTilesetPicker(const TilePicker* p);
        void drawTiles(unsigned int id, const int* tiles, size_t count);
        void drawTile(unsigned int id, int tile, int x, int y, int w, int h);
        void drawBox(int x, int y, int w, int h, Color c, int alpha);
        void drawSelection(int x1, int y1, int x2, int y2);
        void drawGridOverlay();
        void toggleHitboxView();
        void present();
        const static int FRAME_RATE;
        std::map<unsigned int, Texture> textureCache;
        int width;
        int height;
    private:
        SDL_Texture* getFontTexture(const char* text);
        SDL_Texture* getFontTexture(const std::string& text);
        SDL_Window* _window;
        SDL_Renderer* _renderer;
        TTF_Font* _font;
        const char* _resourceFolderPath;
};
#endif
