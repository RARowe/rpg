#ifndef DIALOG_H
#define DIALOG_H
#include <SDL2/SDL.h>
#include "entity.h"

class Dialog
{
    public:
        Dialog(const Entity* player, SDL_Texture* dialogImage);
        ~Dialog();
        void open(const char* imagePath, const char* text);
        void draw(SDL_Renderer* renderer);
    private:
        const Entity* _player;
        SDL_Texture* _dialogImage;
        SDL_Rect _rect = {1, 250, 608, 150};
        bool _isOpen;
};
#endif
