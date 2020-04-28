#pragma once
#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H
#include <memory>
#include <vector>
#include "menu_item.h"
class GameContext;

class PauseMenu
{
    public:
        PauseMenu(GameContext* context);
        void open();
        void cursorUp();
        void cursorDown();
        void click();
        void draw();
        bool isOpen() const;
    private:
        GameContext* _context = nullptr;
        std::vector<std::unique_ptr<MenuItem>> _menuItems;
        bool _isOpen = false;
        int _cursorPosition = 0;
};
#endif