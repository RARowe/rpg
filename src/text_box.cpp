#include <cstring>
#include "text_box.h"

TextBox::TextBox(GraphicsContext* graphics, const Entity* player)
{
    _graphics = graphics;
    _player = player;
    _dialogImage = graphics->getTexture("text_box.png");
}

TextBox::~TextBox()
{
    SDL_DestroyTexture(_dialogImage);
}

void TextBox::setImageAndText(const char* imagePath, const char* text)
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

    _timage = _graphics->getTexture(imagePath);
    _text = _graphics->getFontTexture(textLine1);
    _textLine2 = _graphics->getFontTexture(textLine2);
    _textRect.w = std::strlen(textLine1) * 20;
    _textRectLine2.w = std::strlen(textLine2) * 20;
}

void TextBox::open(const std::vector<const Speech*>* speech)
{
    _textBoxType = TextBoxType::DIALOGUE;
    _isOpen = true;
    _dialogue = speech;
    _dialogueIndex = 0;
    _speechIndex = 0;

}

void TextBox::setNextImageAndText()
{
    if (_dialogueIndex >= _dialogue->size())
    {
        _dialogueIndex = 0;
        _speechIndex = 0;
        _isOpen = false;
        return;
    }

    if (_speechIndex >= (*_dialogue)[_dialogueIndex]->lines.size())
    {
        _dialogueIndex++;
        _speechIndex = 0;
        setNextImageAndText();
    }
    else
    {
        auto s = (*_dialogue)[_dialogueIndex];
        setImageAndText(s->speaker.c_str(), s->lines[_speechIndex].c_str());
        _speechIndex++;
    }
}
void TextBox::open(const char* imagePath, const char* text)
{
    _textBoxType = TextBoxType::SIMPLE;
    _isOpen = true;
    setImageAndText(imagePath, text);
}

void TextBox::draw(SDL_Renderer* renderer)
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

bool TextBox::isOpen()
{
    return _isOpen;
}

void TextBox::processInput(KeyboardHandler& keyboard)
{
    if (keyboard.isPressedAndConsume(SDLK_f))
    {
        if (_textBoxType == TextBoxType::SIMPLE)
        {
            _isOpen = false;
        }
        else
        {
            setNextImageAndText();
        }
    }
}
