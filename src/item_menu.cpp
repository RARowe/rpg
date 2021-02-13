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
    _items.clear();
    for (auto&& i : _context->player->getInventory())
    {
        _items.push_back(i.second);
    }
}

void ItemMenu::moveCursor(CursorMovement m)
{
    _context->getAudio().playSound("audio/menu_navigate.ogg");
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
    if (index < _items.size())
    {
        auto&& item = _items[index];
        _context->openTextBox(TileSets::ITEMS, (int)item.item.texture, item.item.description);
    }
}

void ItemMenu::draw(const TimeStep& timeStep)
{
    auto g = _context->getGraphics();

    g->drawBox(0, 0, 608, 416, Color::BLUE);
    // horizontal border
    g->drawBox(16, 16, 32, 4, Color::WHITE);
    g->drawBox(240, 16, 352, 4, Color::WHITE);
    g->drawBox(16, 396, 576, 4, Color::WHITE);
    // vertical border
    g->drawBox(16, 16, 4, 384, Color::WHITE);
    g->drawBox(588, 16, 4, 384, Color::WHITE);

    g->drawText(64, 0, 32, "Inventory");

    g->drawGrid<InventoryItem>
    (
        32,
        32,
        MAX_Y_INDEX,
        MAX_X_INDEX,
        32,
        32,
        6,
        _items,
        _drawInventoryItem
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
    auto g = context->getGraphics();
    _drawInventoryItem = [g](int x, int y, int w, int h, InventoryItem i)
    {
        g->drawTile(TileSets::ITEMS, (int)i.item.texture, x, y, w, h);
    };

    _drawCursor = [g](int x, int y, int w, int h)
    {
        g->drawTile(TileSets::MISC, (int)MiscSheetTexture::CROSSHAIR, x, y, w, h);
    };
}
