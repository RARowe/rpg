#pragma once
#ifndef MENU_ITEM_H
#define MENU_ITEM_H
#include <string>

class MenuItem
{
    public:
        MenuItem(const std::string& text);
        const std::string& getText();
        void click();
    private:
        std::string _text;
};
#endif