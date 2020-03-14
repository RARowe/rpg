#include "dialog.h"
#include <iostream>

Dialog::Dialog(const Entity* player, SDL_Texture* dialogImage)
{
    _player = player;
    _isOpen = false;
    _dialogImage = dialogImage;
}

Dialog::~Dialog()
{
    SDL_DestroyTexture(_dialogImage);
}

void Dialog::open(const char* imagePath, const char* text)
{
    _isOpen = !_isOpen;
}

void Dialog::draw(SDL_Renderer* renderer)
{
    if (_isOpen)
    {
        int playerY = _player->getY();
        if (playerY > 250)
        {
            _rect.y = 1;
        }
        else if (_rect.y == 1 && playerY < 150)
        {
            _rect.y = 250;
        }
        SDL_RenderCopy(renderer, _dialogImage, NULL, &_rect);
    }
}
