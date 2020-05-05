#include "menus/menu.h"
#include "menus/menu_manager.h"

Menu::Menu(MenuManager* manager) : _manager(manager) { }

void Menu::close()
{
    _manager->closeCurrentMenu();
}