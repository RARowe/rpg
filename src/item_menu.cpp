#include "enums.h"
#include "game_context.h"
#include "menus/item_menu.h"

ItemMenu* ItemMenu::getInstance(GameContext* context, MenuManager* manager)
{
    static ItemMenu menu(context, manager);
    return &menu;
}

void ItemMenu::moveCursor(CursorMovement m)
{
    switch (m)
    {
        case CursorMovement::UP:
            moveUp();
            break;
        case CursorMovement::DOWN:
            moveDown();
            break;
        case CursorMovement::LEFT:
            moveLeft();
            break;
        case CursorMovement::RIGHT:
            moveRight();
            break;
        default:
            break;
    }
}

static inline void playAudio(GameContext* c)
{
    c->getAudio().playSound("audio/menu_navigate.ogg");
}

void ItemMenu::moveUp()
{
    playAudio(_context);
    _cursorY--;
    if (_cursorY < MIN_Y_INDEX) { _cursorY = MAX_Y_INDEX; }
}

void ItemMenu::moveDown()
{
    playAudio(_context);
    _cursorY++;
    if (_cursorY > MAX_Y_INDEX) { _cursorY = MIN_Y_INDEX; }
}

void ItemMenu::moveLeft()
{
    playAudio(_context);
    _cursorX--;
    if (_cursorX < MIN_X_INDEX) { _cursorX = MAX_X_INDEX; }
}

void ItemMenu::moveRight()
{
    playAudio(_context);
    _cursorX++;
    if (_cursorX > MAX_X_INDEX) { _cursorX = MIN_X_INDEX; }
}

void ItemMenu::click()
{

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

    int x = 32;
    int y = 32;
    for (auto&& i : _context->getPlayer()->getInventory())
    {
        auto&& item = i.second.item;
        g->drawTile(TileSets::ITEMS, (int)item.texture, x, y, 32, 32);
        // we want 12px margin
        x += 44;
    }
    
    g->drawTile(TileSets::MISC, (int)MiscSheetTexture::CROSSHAIR, _cursorX * 32 + (_cursorX - 1) * 12, _cursorY * 32, 32, 32);
}

ItemMenu::ItemMenu(GameContext* context, MenuManager* manager) : Menu(manager),  _context(context) { }