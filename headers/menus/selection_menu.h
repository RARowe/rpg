#pragma once
#ifndef SELECTION_MENU_H
#define SELECTION_MENU_H
#include <memory>
#include <vector>
#include "menus/menu.h"
#include "menus/menu_manager.h"
#include "menu_item.h"
#include "time_step.h"

class GameContext;

class SelectionMenu : public Menu
{
    public:
        static Menu* getInstance(GameContext* context, MenuManager* manager);
        void init();
        void moveCursor(CursorMovement m);
        void click();
        void draw(const TimeStep& timeStep);
    private:
        SelectionMenu(GameContext* context, MenuManager* manager);
        GameContext* _context = nullptr;
        std::vector<std::unique_ptr<MenuItem>> _menuItems;
        int _cursorPosition = 0;
};
#endif