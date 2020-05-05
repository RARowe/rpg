#include "game_context.h"
#include "menus/menu_manager.h"
#include "menus/item_menu.h"
#include "menus/pause_menu.h"

MenuManager* MenuManager::getInstance(GameContext* context)
{
    static MenuManager manager(context);
    return &manager;
}

void MenuManager::open(MenuType type)
{
    Menu* menu;
    switch (type)
    {
        case MenuType::PAUSE:
            menu = _pauseMenu;
            break;
        case MenuType::ITEM:
            menu = _itemMenu;
            break;
        default:
            break;
    }

    menu->init();
    _menuStack.push(menu);
}

void MenuManager::closeCurrentMenu()
{
    _menuStack.pop();
}

void MenuManager::closeAllMenus()
{
    while (!_menuStack.empty())
    {
        _menuStack.pop();
    }
}

void MenuManager::draw(const TimeStep& timeStep)
{
    if (!_menuStack.empty())
    {
        _menuStack.top()->draw(timeStep);
    }
}

void MenuManager::doAction(MenuAction a)
{
    if (!_menuStack.empty())
    {
        auto activeMenu = _menuStack.top();
        switch (a)
        {
            case MenuAction::MOVE_CURSOR_UP:
                activeMenu->moveCursor(CursorMovement::UP);
                break;
            case MenuAction::MOVE_CURSOR_DOWN:
                activeMenu->moveCursor(CursorMovement::DOWN);
                break;
            case MenuAction::MOVE_CURSOR_LEFT:
                activeMenu->moveCursor(CursorMovement::LEFT);
                break;
            case MenuAction::MOVE_CURSOR_RIGHT:
                activeMenu->moveCursor(CursorMovement::RIGHT);
                break;
            case MenuAction::CURSOR_CLICK:
                activeMenu->click();
                break;
            default:
                break;
        }
    }
}

MenuManager::MenuManager(GameContext* context)
{
    _pauseMenu = PauseMenu::getInstance(context, this);
    _itemMenu = ItemMenu::getInstance(context, this);
}