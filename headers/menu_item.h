#pragma once
#ifndef MENU_ITEM_H
#define MENU_ITEM_H
#include <functional>
#include <string>

class GameContext;

class MenuItem
{
    public:
        MenuItem(std::function<void ()> onClick, const std::string& text);
        const std::string& getText();
        void click();
    private:
        std::function<void ()> _onClick;
        std::string _text;
};
#endif