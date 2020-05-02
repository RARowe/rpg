#ifndef TEXT_BOX_H
#define TEXT_BOX_H
#include "entity.h"
#include "enums.h"
#include "graphics_context.h"
#include "keyboard_handler.h"
#include "types.h"

class TextBox
{
    public:
        TextBox(GraphicsContext* graphics, const Entity* player);
        void open(const char* imagePath, const char* text);
        void open(const std::vector<const Speech*>* script);
        void open(TileSets tileSet, int tile, const std::string& text);
        void draw();
        bool isOpen();
        void click();
    private:
        void setNextImageAndText();
        void setImageAndText(const char* imagePath, const char* text);
        GraphicsContext* _graphics;
        const Entity* _player;
        TileSets _tileSet;
        int _tile;
        const char* _imagePath = nullptr;
        std::string _text;
        TextBoxType _textBoxType = TextBoxType::SIMPLE;
        const std::vector<const Speech*>* _dialogue = nullptr;
        int _dialogueIndex = 0;
        int _speechIndex = 0;
        bool _isOpen = false;
};
#endif
