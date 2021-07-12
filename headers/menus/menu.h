#pragma once
#ifndef MENU_H
#define MENU_H
class MenuManager;

enum class CursorMovement
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Menu
{
    public:
        Menu(MenuManager* manager);
        virtual void init() = 0;
        virtual void moveCursor(CursorMovement m) = 0;
        virtual void click() = 0;
        virtual void close() final;
        virtual void draw(const float timeStep) = 0;
    private:
        MenuManager* _manager;
};
#endif
