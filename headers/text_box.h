#ifndef TEXT_BOX_H
#define TEXT_BOX_H
#include <SDL2/SDL.h>
#include "entity.h"
#include "graphics_context.h"
#include "keyboard_handler.h"

class TextBox
{
    public:
        TextBox(GraphicsContext* graphics, const Entity* player);
        ~TextBox();
        void open(const char* imagePath, const char* text);
        void draw(SDL_Renderer* renderer);
        bool isOpen();
        void processInput(KeyboardHandler& keyboard);
    private:
        GraphicsContext* _graphics;
        const Entity* _player;
        SDL_Texture* _dialogImage;
        SDL_Texture* _timage = NULL;
        SDL_Texture* _text = NULL;
        SDL_Texture* _textLine2 = NULL;
        SDL_Rect _rect = {0, 266, 608, 150};
        SDL_Rect _timRect = { 0, 266, 150, 150 };
        SDL_Rect _textRect = { 160, 276, 200, 24 };
        SDL_Rect _textRectLine2 = { 160, 310, 200, 24 };
        bool _isOpen = false;
};
#endif
