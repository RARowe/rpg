#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>
#include <vector>
#include "entity.h"
class GraphicsContext
{
    public:
        GraphicsContext(const char* title, int width, int height, const char* resourceFolderPath);
        ~GraphicsContext();
        SDL_Window* getWindow();
        SDL_Renderer* getRenderer();
        SDL_Texture* getTexture(const char* path) const;
        void drawTexture(const Entity& e, const std::string& name);
        void drawTexture(int x, int y, int w, int h, const std::string& name);
        void drawText(int x, int y, int w, int h, const char* text);
        void drawText(int x, int y, int fontSize, const std::string& text);
        void drawWrappedText(int x, int y, int fontSize, int maxWidth, const std::string& text);
        void drawEmote(const Entity& e, const std::string& name);
        void drawAbove(const Entity& e, TileSets tileSet, int tile);
        void drawTiles(TileSets t, const std::vector<int>& positions);
        void drawTile(TileSets tileSet, int tile, int x, int y, int w, int h);
        void drawHitbox(int x, int y, int w, int h);
        void drawBox(int x, int y, int w, int h, Color c);
        template <class T>
        void drawGrid
        (
            int x,
            int y,
            int numberOfRows,
            int numberOfColumns,
            int cellWidth,
            int cellHeight,
            int cellMargin,
            std::vector<T>& entities,
            std::function<void (int, int, int, int, T&)> drawFunction
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
        int getWidth();
        int getHeight();
        const static int FRAME_RATE;
    private:
        SDL_Texture* getFontTexture(const char* text);
        SDL_Texture* getFontTexture(const std::string& text);
        SDL_Window* _window;
        SDL_Renderer* _renderer;
        TTF_Font* _font;
        SDL_Texture* _emoteSheet = nullptr;
        std::map<const std::string, SDL_Texture*> _textureCache;
        const char* _resourceFolderPath;
        int _width;
        int _height;
        bool _showHitboxes = false;
};

template <class T>
void GraphicsContext::drawGrid
(
    int x,
    int y,
    int numberOfRows,
    int numberOfColumns,
    int cellWidth,
    int cellHeight,
    int cellMargin,
    std::vector<T>& entities,
    std::function<void (int, int, int, int, T&)> drawFunction
)
{
    int xIncrementValue = x + cellMargin;
    int yIncrementValue = y + cellMargin;
    int newX = xIncrementValue;
    int newY = yIncrementValue;
    int row = 1;
    int column = 1;

    for (auto&& e : entities)
    {
        drawFunction(newX, newY, cellWidth, cellHeight, e);
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
#endif
