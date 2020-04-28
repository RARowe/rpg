#include <iostream>
#include "menu_item.h"

MenuItem::MenuItem(const std::string& text) : _text(text) { }

const std::string& MenuItem::getText()
{
    return _text;
}

void MenuItem::click()
{
    std::cout << "Clicked: " << _text << std::endl;
}