#include "enums.h"
#include "game_context.h"
#include "menus/item_menu.h"

ItemMenu* ItemMenu::getInstance(GameContext* context, MenuManager* manager)
{
    static ItemMenu menu(context, manager);
    return &menu;
}

void ItemMenu::init()
{
    _cursorX = 1;
    _cursorY = 1;
}

void ItemMenu::moveCursor(CursorMovement m)
{
    _context->audio.playSound("resources/audio/menu_navigate.ogg");
    switch (m)
    {
        case CursorMovement::UP:
            _cursorY--;
            if (_cursorY < MIN_Y_INDEX) { _cursorY = MAX_Y_INDEX; }
            break;
        case CursorMovement::DOWN:
            _cursorY++;
            if (_cursorY > MAX_Y_INDEX) { _cursorY = MIN_Y_INDEX; }
            break;
        case CursorMovement::LEFT:
            _cursorX--;
            if (_cursorX < MIN_X_INDEX) { _cursorX = MAX_X_INDEX; }
            break;
        case CursorMovement::RIGHT:
            _cursorX++;
            if (_cursorX > MAX_X_INDEX) { _cursorX = MIN_X_INDEX; }
            break;
        default:
            break;
    }
}

void ItemMenu::click()
{
    int x = _cursorX - 1;
    int y = ((_cursorY - 1) * MAX_Y_INDEX);
    int index = x + y;
    if (index < _context->inventory->size)
    {
        //auto&& itemType = _context->inventory->items[index];
        //_context->requestOpenTextBox(
        //        TileSets::ITEMS,
        //        (int)inventory_get_item_texture(itemType),
        //        inventory_get_item_description(itemType));
    }
}

void ItemMenu::draw(const float timeStep)
{
    auto g = _context->graphics;

    g->drawBox(0, 0, 608, 416, Color::BLUE, 255);
    // horizontal border
    g->drawBox(16, 16, 32, 4, Color::WHITE, 255);
    g->drawBox(240, 16, 352, 4, Color::WHITE, 255);
    g->drawBox(16, 396, 576, 4, Color::WHITE, 255);
    // vertical border
    g->drawBox(16, 16, 4, 384, Color::WHITE, 255);
    g->drawBox(588, 16, 4, 384, Color::WHITE, 255);

    g->drawText(64, 0, 32, "Inventory");

    g->drawInventory
    (
        32,
        32,
        MAX_Y_INDEX,
        MAX_X_INDEX,
        32,
        32,
        6,
        _context->inventory
    );

    g->drawOnGridAt
    (
        32,
        32,
        32,
        32,
        6,
        _cursorY,
        _cursorX,
        _drawCursor
    );
}


ItemMenu::ItemMenu(GameContext* context, MenuManager* manager) : Menu(manager),  _context(context) 
{
    //auto g = context->graphics;

    _drawCursor = [](int x, int y, int w, int h)
    {
        //g->drawTile(TileSets::MISC, (int)MiscSheetTexture::CROSSHAIR, x, y, w, h);
    };
}
