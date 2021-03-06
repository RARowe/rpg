#include <memory>
#include <functional>
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

void MenuManager::open(Menu* menu)
{
    menu->init();
    _menuStack.push(menu);
}

void MenuManager::closeCurrentMenu()
{
    _menuStack.pop();

    if (_menuStack.empty())
    {
        _context->onAllMenusClosed();
    }
}

void MenuManager::closeAllMenus()
{
    while (!_menuStack.empty())
    {
        _menuStack.pop();
    }
    _context->onAllMenusClosed();
}

void MenuManager::draw(const float timeStep)
{
    if (!_menuStack.empty())
    {
        _menuStack.top()->draw(timeStep);
    }
}

void MenuManager::processInput(const PlayerInput* input) {
    if (input->pause)
    {
        closeAllMenus();
    }
    else
    {
        if (input->upClick)
        {
            doAction(MenuAction::MOVE_CURSOR_UP);
        }
        else if (input->downClick)
        {
            doAction(MenuAction::MOVE_CURSOR_DOWN);
        }
        else if (input->leftClick)
        {
            doAction(MenuAction::MOVE_CURSOR_LEFT);
        }
        else if (input->rightClick)
        {
            doAction(MenuAction::MOVE_CURSOR_RIGHT);
        }

        if (input->select)
        {
            doAction(MenuAction::CURSOR_CLICK);
        }

        if (input->back)
        {
            closeCurrentMenu();
        }
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

MenuManager::MenuManager(GameContext* context) : _context(context)
{
    _pauseMenu = PauseMenu::getInstance(context, this);
    _itemMenu = ItemMenu::getInstance(context, this);
}
