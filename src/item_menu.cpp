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

void ItemMenu::moveUp()
{
    _cursorY++;
    if (_cursorY == MAX_Y_INDEX) { _cursorY = 0; }
}

void ItemMenu::moveDown()
{
    _cursorY--;
    if (_cursorY < 0) { _cursorY = MAX_Y_INDEX; }
}

void ItemMenu::moveLeft()
{
    _cursorX--;
    if (_cursorX < 0) { _cursorX = MAX_X_INDEX; }
}

void ItemMenu::moveRight()
{
    _cursorX++;
    if (_cursorX == MAX_X_INDEX) { _cursorX = 0; }
}

void ItemMenu::click()
{

}

void ItemMenu::draw(const float timeStep)
{
    auto g = _context->getGraphics();

    g->drawBox(0, 0, 608, 416, Color::BLUE);
    // horizontal border
    g->drawBox(16, 16, 576, 4, Color::WHITE);
    g->drawBox(16, 396, 576, 4, Color::WHITE);
    // vertical border
    g->drawBox(16, 16, 4, 384, Color::WHITE);
    g->drawBox(588, 16, 4, 384, Color::WHITE);
}

ItemMenu::ItemMenu(GameContext* context, MenuManager* manager) : Menu(manager),  _context(context) { }