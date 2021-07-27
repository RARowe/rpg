#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>
#include <memory>
#include "types.h"

enum class WindowPosition
{
    INSIDE,
    ABOVE,
    BELOW,
    LEFT,
    RIGHT
};

typedef struct {
    SDL_Texture* texture;
    char name[64];
    unsigned int w, h;
} Texture;

class GraphicsContext
{
    public:
        GraphicsContext(const char* title, int width, int height, const char* resourceFolderPath);
        ~GraphicsContext();
        void drawText(int x, int y, int w, int h, const char* text);
        void drawText(int x, int y, int fontSize, const std::string& text);
        void drawWrappedText(int x, int y, int fontSize, int maxWidth, const std::string& text);
        void drawTexture(unsigned int id, int x, int y, int w, int h);
        void drawTiles(unsigned int id, const int* tiles, size_t count);
        void drawTile(unsigned int id, int tile, int x, int y, int w, int h);
        void drawHitbox(int x, int y, int w, int h);
        void drawBox(int x, int y, int w, int h, Color c, int alpha);
        void drawSelection(int x1, int y1, int x2, int y2);
        void drawGridOverlay();
        // TODO: This should not exist... Inventory items are simple sprites
        void drawInventory
        (
            int x,
            int y,
            int numberOfRows,
            int numberOfColumns,
            int cellWidth,
            int cellHeight,
            int cellMargin,
            Inventory* inventory
        );
        void drawOnGridAt
        (
            int x,
            int y,
            int cellWidth,
            int cellHeight,
            int cellMargin,
            int row,
            int column,
            std::function<void (int, int, int, int)> drawFunction
        );
        void toggleHitboxView();
        void present();
        WindowPosition getPosition(int x, int y) const;
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
        bool _showHitboxes = false;
};
#endif
