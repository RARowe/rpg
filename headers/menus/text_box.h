#ifndef TEXT_BOX_H
#define TEXT_BOX_H
#include "entity.h"
#include "enums.h"
#include "graphics_context.h"
#include "menus/menu.h"
#include "menus/menu_manager.h"
#include "time_step.h"
#include "types.h"

class TextBox : public Menu
{
    public:
        TextBox(GraphicsContext* graphics, const Entity* player, MenuManager* manager);
        void open(const char* imagePath, const char* text);
        void open(Speech script);
        // TODO: Reduce these
        void open(TileSets tileSet, int tile, const char* text);
        void open(TileSets tileSet, int tile, const std::string& text);
        void draw(const TimeStep& timeStep);
        void init();
        void moveCursor(CursorMovement m);
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
        Speech _dialogue;
};
#endif
