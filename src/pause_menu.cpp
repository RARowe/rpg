#include <cmath>
#include "game_context.h"
#include "menus/pause_menu.h"

static std::function<void ()> volumeUp(GameContext* c)
{
    return [c]()
    {
        c->getAudio().volumeUp();
    };
}

static std::function<void ()> volumeDown(GameContext* c)
{
    return [c]()
    {
        c->getAudio().volumeDown();
    };
}

static std::function<void ()> inventory(GameContext* c)
{
    return [c]()
    { 
        c->openMenu(MenuType::ITEM);
    };
}

static std::function<void ()> back(Menu* m)
{
    return [m]()
    {
        m->close();
    };
}

PauseMenu* PauseMenu::getInstance(GameContext* context, MenuManager* manager)
{
    static PauseMenu pause(context, manager);
    return &pause;
}

PauseMenu::PauseMenu(GameContext* context, MenuManager* manager) : Menu(manager), _context(context)
{
    _menuItems.push_back(std::make_unique<MenuItem>(MenuItem(volumeUp(context), "Volume up")));
    _menuItems.push_back(std::make_unique<MenuItem>(MenuItem(volumeDown(context), "Volume down")));
    _menuItems.push_back(std::make_unique<MenuItem>(MenuItem(inventory(context), "Inventory")));
    _menuItems.push_back(std::make_unique<MenuItem>(MenuItem(back(this), "Back")));
}

void PauseMenu::init()
{
    _context->getAudio().playPauseMenuMusic(true);
    _cursorPosition = 0;
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
    _menuItems[_cursorPosition]->click();
}

void PauseMenu::moveCursor(CursorMovement m)
{
    switch (m)
    {
        case CursorMovement::UP:
            cursorUp();
            break;
        case CursorMovement::DOWN:
            cursorDown();
            break;
        default:
            break;
    }
}

void PauseMenu::draw(const TimeStep& timeStep)
{
    auto g = _context->getGraphics();
    g->drawBox(11 * 32, 0, 8 * 32, 13 * 32, Color::BLUE);

    int y = 0;
    int index = 0;
    for (auto const& m : _menuItems)
    {
        float junk, integerPlace;
        if (index == _cursorPosition)
        {
            g->drawText(11 * 32, y, 32, ">");
        }
        g->drawText(12 * 32, y, 32, m->getText());
        y += 32;
        index++;
    }
}