#pragma once
#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H
class GameContext;

class PauseMenu
{
    public:
        PauseMenu(GameContext* context);
        void open();
        void draw();
        bool isOpen() const;
    private:
        GameContext* _context = nullptr;
        bool _isOpen = false;
};
#endif