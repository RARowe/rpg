#pragma once
#ifndef MENU_ITEM_H
#define MENU_ITEM_H
#include <functional>
#include <string>

class GameContext;

class MenuItem
{
    public:
        MenuItem(std::function<void (GameContext&)> onClick, const std::string& text);
        const std::string& getText();
        void click(GameContext& context);
    private:
        std::function<void (GameContext&)> _onClick;
        std::string _text;
};
#endif