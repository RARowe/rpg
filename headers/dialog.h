#ifndef DIALOG_H
#define DIALOG_H
#include <SDL2/SDL.h>
#include "entity.h"
#include "graphics_context.h"
#include "keyboard_handler.h"

class Dialog
{
    public:
        Dialog(GraphicsContext* graphics, const Entity* player);
        ~Dialog();
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
        SDL_Rect _rect = {0, 266, 608, 150};
        SDL_Rect _timRect = { 0, 266, 150, 150 };
        SDL_Rect _textRect = { 160, 276, 200, 24 };
        bool _isOpen = false;
};
#endif
