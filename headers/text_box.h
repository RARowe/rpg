#ifndef TEXT_BOX_H
#define TEXT_BOX_H
#include <SDL2/SDL.h>
#include "entity.h"
#include "enums.h"
#include "graphics_context.h"
#include "keyboard_handler.h"
#include "types.h"

class TextBox
{
    public:
        TextBox(GraphicsContext* graphics, const Entity* player);
        ~TextBox();
        void open(const char* imagePath, const char* text);
        void open(const std::vector<const Speech*>* script);
        void draw(SDL_Renderer* renderer);
        bool isOpen();
        void click();
    private:
        void setNextImageAndText();
        void setImageAndText(const char* imagePath, const char* text);
        GraphicsContext* _graphics;
        const Entity* _player;
        SDL_Texture* _timage = NULL;
        SDL_Texture* _text = NULL;
        SDL_Texture* _textLine2 = NULL;
        SDL_Rect _rect = {0, 266, 608, 150};
        SDL_Rect _timRect = { 0, 266, 150, 150 };
        SDL_Rect _textRect = { 160, 276, 200, 24 };
        SDL_Rect _textRectLine2 = { 160, 310, 200, 24 };
        TextBoxType _textBoxType = TextBoxType::SIMPLE;
        const std::vector<const Speech*>* _dialogue = nullptr;
        int _dialogueIndex = 0;
        int _speechIndex = 0;
        bool _isOpen = false;
};
#endif
