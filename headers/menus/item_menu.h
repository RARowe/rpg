#pragma once
#ifndef ITEM_MENU_H
#define ITEM_MENU_H
#include <functional>
#include <memory>
#include "items.h"
#include "menus/menu.h"
#include "menus/menu_manager.h"
#include "time_step.h"

// 0 indexed
#define MIN_X_INDEX 1
#define MIN_Y_INDEX 1
#define MAX_X_INDEX 14
#define MAX_Y_INDEX 8

class GameContext;

class ItemMenu : public Menu
{
    public:
        static ItemMenu* getInstance(GameContext* context, MenuManager* manager);
        void init();
        void moveCursor(CursorMovement m);
        void click();
        void draw(const TimeStep& timeStep);
    private:
        ItemMenu(GameContext* context, MenuManager* manager);
        void moveUp();
        void moveDown();
        void moveLeft();
        void moveRight();
        GameContext* _context;
        std::vector<InventoryItem> _items;
        std::function<void (int, int, int, int, InventoryItem)> _drawInventoryItem;
        std::function<void (int, int, int, int)> _drawCursor;
        int _cursorX = 1;
        int _cursorY = 1;
};
#endif