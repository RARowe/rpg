#pragma once
#ifndef ITEM_MENU_H
#define ITEM_MENU_H
#include <memory>
#include "menus/menu.h"
#include "menus/menu_manager.h"
#include "time_step.h"

// 0 indexed
#define MIN_X_INDEX 1
#define MIN_Y_INDEX 1
#define MAX_X_INDEX 17
#define MAX_Y_INDEX 11

class GameContext;

class ItemMenu : public Menu
{
    public:
        static ItemMenu* getInstance(GameContext* context, MenuManager* manager);
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
        int _cursorX = 1;
        int _cursorY = 1;
};
#endif