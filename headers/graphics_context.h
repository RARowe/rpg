#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include "types.h"

typedef struct {
    SDL_Texture* texture;
    int columns;
    int rows;
    int spriteWidth;
    int spriteHeight;
} SpriteData;

enum class WindowPosition
{
    INSIDE,
    ABOVE,
    BELOW,
    LEFT,
    RIGHT
};

class GraphicsContext
{
    public:
        GraphicsContext(const char* title, int width, int height, const char* resourceFolderPath);
        ~GraphicsContext();
        void drawTexture(const Body& b, const std::string& name);
        void drawTexture(int x, int y, int w, int h, const std::string& name);
        void drawText(int x, int y, int w, int h, const char* text);
        void drawText(int x, int y, int fontSize, const std::string& text);
        void drawWrappedText(int x, int y, int fontSize, int maxWidth, const std::string& text);
        void drawTiles(TileSets t, const std::vector<int>& positions);
        void drawTile(TileSets tileSet, int tile, int x, int y, int w, int h);
        void drawHitbox(int x, int y, int w, int h);
        void drawBox(int x, int y, int w, int h, Color c, int alpha);
        void drawSelection(int x1, int y1, int x2, int y2);
        void drawSprite(const std::string& spriteSheet, int sprite, const Body& b);
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
    private:
        SDL_Texture* getTexture(const std::string& path);
        SDL_Texture* getFontTexture(const char* text);
        SDL_Texture* getFontTexture(const std::string& text);
        const SpriteData& getSpriteData(const std::string& spriteSheet);
        SDL_Window* _window;
        SDL_Renderer* _renderer;
        TTF_Font* _font;
        SDL_Texture* _emoteSheet = nullptr;
        std::map<const std::string, SDL_Texture*> _textureCache;
        std::map<const std::string, std::unique_ptr<SpriteData>> _spriteCache;
        const char* _resourceFolderPath;
        int _width;
        int _height;
        bool _showHitboxes = false;
};
#endif
