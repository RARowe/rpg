#include "dialog.h"

Dialog::Dialog(GraphicsContext* graphics, const Entity* player)
{
    _graphics = graphics;
    _player = player;
    _dialogImage = graphics->getTexture("text_box.png");
}

Dialog::~Dialog()
{
    SDL_DestroyTexture(_dialogImage);
}

void Dialog::open(const char* imagePath, const char* text)
{
    _isOpen = !_isOpen;
    _timage = _graphics->getTexture(imagePath);
    _text = _graphics->getFontTexture(text);
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

bool Dialog::isOpen()
{
    return _isOpen;
}

void Dialog::processInput(KeyboardHandler& keyboard)
{
    if (keyboard.isPressedAndConsume(SDLK_f))
    {
        _isOpen = false;
    }
}
