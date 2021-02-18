#include "menus/text_box.h"

TextBox::TextBox(GraphicsContext* graphics, const Entity* player, MenuManager* manager) : Menu(manager)
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
    _dialogue = speech;
    _dialogueIndex = 0;
    _speechIndex = 0;
}

// TODO: Remove this
void TextBox::open(TileSets t, int tile, const char* text)
{
    _textBoxType = TextBoxType::SIMPLE_WITH_TILESET;
    _tileSet = t;
    _tile = tile;
    _text = text;
}

void TextBox::open(TileSets t, int tile, const std::string& text)
{
    _textBoxType = TextBoxType::SIMPLE_WITH_TILESET;
    _tileSet = t;
    _tile = tile;
    _text = text;
}

void TextBox::setNextImageAndText()
{
    if (_dialogueIndex >= _dialogue->size())
    {
        _dialogueIndex = 0;
        _speechIndex = 0;
        close();
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
    setImageAndText(imagePath, text);
}

void TextBox::draw(const TimeStep& timeStep)
{
    int playerY = _player->pos.y;
    int y = playerY > 256 ? 0 : 256;
    _graphics->drawBox(0, y, 608, 160, Color::BLUE);
    if (_textBoxType != TextBoxType::SIMPLE_WITH_TILESET)
    {
        _graphics->drawTexture(0, y, 160, 160, _imagePath);
    }
    else
    {
        _graphics->drawTile(_tileSet, _tile, 0, y, 160, 160);
    }
    _graphics->drawWrappedText(192, y, 32, 384, _text);
}

void TextBox::click()
{
    if (_textBoxType != TextBoxType::DIALOGUE)
    {
        close();
    }
    else
    {
        setNextImageAndText();
    }
}

void TextBox::init() { }

void TextBox::moveCursor(CursorMovement m) { }
