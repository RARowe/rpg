#pragma once
#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H
#include <stack>
#include "menus/menu.h"
#include "time_step.h"

class GameContext;

enum class MenuType
{
    PAUSE,
    ITEM
};

enum class MenuAction
{
    MOVE_CURSOR_UP,
    MOVE_CURSOR_DOWN,
    MOVE_CURSOR_LEFT,
    MOVE_CURSOR_RIGHT,
    CURSOR_CLICK
};

class MenuManager
{
    public:
        static MenuManager* getInstance(GameContext* context);
        void open(MenuType t);
        void open(Menu* m);
        void closeCurrentMenu();
        void closeAllMenus();
        void draw(const TimeStep& timeStep);
        void doAction(MenuAction a);
    private:
        MenuManager(GameContext* context);
        GameContext* _context = nullptr;
        std::stack<Menu*> _menuStack;
        Menu* _pauseMenu = nullptr;
        Menu* _itemMenu = nullptr;
};
#endif