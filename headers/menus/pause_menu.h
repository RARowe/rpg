#pragma once
#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H
#include <memory>
#include <vector>
#include "menus/menu.h"
#include "menus/menu_manager.h"
#include "menu_item.h"
class GameContext;

class PauseMenu : public Menu
{
    public:
        static PauseMenu* getInstance(GameContext* context, MenuManager* manager);
        void moveCursor(CursorMovement m);
        void click();
        void draw(const float timeStep);
    private:
        PauseMenu(GameContext* context, MenuManager* manager);
        void cursorUp();
        void cursorDown();
        GameContext* _context = nullptr;
        std::vector<std::unique_ptr<MenuItem>> _menuItems;
        int _cursorPosition = 0;
};
#endif