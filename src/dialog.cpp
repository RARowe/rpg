#include "dialog.h"

Dialog::Dialog(GraphicsContext* graphics, const Entity* player)
{
    _graphics = graphics;
    _player = player;
    _dialogImage = graphics->getTexture("text_box.png");
    _timage = graphics->getTexture("tim.png");
    _text = graphics->getFontTexture("it trash");
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
        if (playerY > 266)
        {
            _rect.y = 1;
            _timRect.y = 1;
            _textRect.y = 10;
        }
        else if (_rect.y == 1 && playerY < 150)
        {
            _rect.y = 266;
            _timRect.y = 266;
            _textRect.y = 276;
        }
        SDL_RenderCopy(renderer, _dialogImage, NULL, &_rect);
        SDL_RenderCopy(renderer, _timage, NULL, &_timRect);
        SDL_RenderCopy(renderer, _text, NULL, &_textRect);
    }
}
