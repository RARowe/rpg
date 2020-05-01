#include "text_box.h"

TextBox::TextBox(GraphicsContext* graphics, const Entity* player)
{
    _graphics = graphics;
    _player = player;
}

void TextBox::setImageAndText(const char* imagePath, const char* text)
{
    _imagePath = imagePath;
    _text = text;
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

void TextBox::draw()
{
    if (_isOpen)
    {
        int playerY = _player->getY();
        int y = playerY > 256 ? 0 : 256;
        _graphics->drawBox(0, y, 608, 160, 48, 72, 203);
        _graphics->drawTexture(0, y, 160, 160, _imagePath);
        _graphics->drawWrappedText(192, y, 32, 384, _text);
    }
}

bool TextBox::isOpen()
{
    return _isOpen;
}

void TextBox::click()
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