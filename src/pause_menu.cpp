#include "pause_menu.h"
#include "game_context.h"
#include <iostream>

static void volumeUp(GameContext& c)
{
    c.getAudio().volumeUp();
}

static void volumeDown(GameContext& c)
{
    c.getAudio().volumeDown();
}

static void inventory(GameContext& c)
{
    for (auto const& i : c.getPlayer()->getInventory())
    {
        std::cout << i.second.item.description << std::endl;
    }
}

static void back(GameContext& c)
{
    c.pause();
}

PauseMenu::PauseMenu(GameContext* context) : _context(context)
{
    _menuItems.push_back(std::make_unique<MenuItem>(MenuItem(volumeUp, "Volume up")));
    _menuItems.push_back(std::make_unique<MenuItem>(MenuItem(volumeDown, "Volume down")));
    _menuItems.push_back(std::make_unique<MenuItem>(MenuItem(inventory, "Inventory")));
    _menuItems.push_back(std::make_unique<MenuItem>(MenuItem(back, "Back")));
}

void PauseMenu::open()
{
    _isOpen = !_isOpen;
    _cursorPosition = 0;
    _context->getAudio().playPauseMenuMusic(_isOpen);
}

void PauseMenu::cursorDown()
{
    _cursorPosition++;
    if (_cursorPosition == _menuItems.size())
    {
        _cursorPosition = 0;
    }
    _context->getAudio().playSound("audio/menu_navigate.ogg");
}

void PauseMenu::cursorUp()
{
    _cursorPosition--;
    if (_cursorPosition < 0)
    {
        _cursorPosition = _menuItems.size() - 1;
    }
    _context->getAudio().playSound("audio/menu_navigate.ogg");
}

void PauseMenu::click()
{
    _menuItems[_cursorPosition]->click(*_context);
}

void PauseMenu::draw()
{
    if (_isOpen)
    {
        auto g = _context->getGraphics();
        g->drawBox(11 * 32, 0, 8 * 32, 13 * 32, 48, 72, 203);

        int y = 0;
        int index = 0;
        for (auto const& m : _menuItems)
        {
            if (index == _cursorPosition)
            {
                g->drawText(11 * 32, y, 32, ">");
            }
            g->drawText(12 * 32, y, 32, m->getText());
            y += 32;
            index++;
        }
    }
}

bool PauseMenu::isOpen() const
{
    return _isOpen;
}