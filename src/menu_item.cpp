#include "game_context.h"
#include "menu_item.h"

MenuItem::MenuItem(std::function<void (GameContext&)> onClick, const std::string& text) : _onClick(onClick), _text(text) { }

const std::string& MenuItem::getText()
{
    return _text;
}

void MenuItem::click(GameContext& context)
{
    _onClick(context);
}