#include <cstring>
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
    // TODO: This could be a lot cleaner
    char textLine1[21] = "";
    char textLine2[21] = "";
    size_t length = std::strlen(text);

    std::strncpy(textLine1, text, 20);

    // adding 1 for null terminator
    if (length + 1 > 21)
    {
        std::strncpy(textLine2, text + 20, 20);
    }

    _isOpen = !_isOpen;
    _timage = _graphics->getTexture(imagePath);
    _text = _graphics->getFontTexture(textLine1);
    _textLine2 = _graphics->getFontTexture(textLine2);
    _textRect.w = std::strlen(textLine1) * 20;
    _textRectLine2.w = std::strlen(textLine2) * 20;
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
            _textRectLine2.y = 40;
        }
        else if (_rect.y == 1 && playerY < 150)
        {
            _rect.y = 266;
            _timRect.y = 266;
            _textRect.y = 276;
            _textRectLine2.y = 310;
        }
        SDL_RenderCopy(renderer, _dialogImage, NULL, &_rect);
        SDL_RenderCopy(renderer, _timage, NULL, &_timRect);
        SDL_RenderCopy(renderer, _text, NULL, &_textRect);
        SDL_RenderCopy(renderer, _textLine2, NULL, &_textRectLine2);
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
