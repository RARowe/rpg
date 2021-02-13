#include "menus/selection_menu.h"
#include "game_context.h"

Menu* SelectionMenu::getInstance(GameContext* context, MenuManager* manager)
{
    static SelectionMenu selectionMenu(context, manager);
    return &selectionMenu;
}

void SelectionMenu::init()
{
    _cursorPosition = 0;
}

void SelectionMenu::moveCursor(CursorMovement m)
{
    switch (m)
    {
        case CursorMovement::UP:
            _cursorPosition--;
            _context->getAudio().playSound("audio/menu_navigate.ogg");
            if (_cursorPosition < 0) { _cursorPosition = _menuItems.size() - 1; }
            break;
        case CursorMovement::DOWN:
            _cursorPosition++;
            _context->getAudio().playSound("audio/menu_navigate.ogg");
            if (_cursorPosition > (_menuItems.size() - 1)) { _cursorPosition = 0; }
            break;
        default:
            break;
    }
}
void SelectionMenu::click()
{
    _menuItems[_cursorPosition]->click();
}

void SelectionMenu::draw(const TimeStep& timeStep)
{
    auto g = _context->getGraphics();
    g->drawBox(11 * 32, 0, 8 * 32, 13 * 32, Color::WHITE);
}

SelectionMenu::SelectionMenu(GameContext* context, MenuManager* manager) : Menu(manager), _context(context)
{
    _menuItems.push_back(std::make_unique<MenuItem>(MenuItem(
        [](){},
        "Save"
    )));
    _menuItems.push_back(std::make_unique<MenuItem>(MenuItem(
        [](){},
        "Tip"
    )));
    _menuItems.push_back(std::make_unique<MenuItem>(MenuItem(
        [](){},
        "Life advice"
    )));
    _menuItems.push_back(std::make_unique<MenuItem>(MenuItem(
        [manager]() { manager->closeCurrentMenu(); },
        "Never mind"
    )));
}