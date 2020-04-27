#include "pause_menu.h"
#include "game_context.h"

PauseMenu::PauseMenu(GameContext* context) : _context(context) { }

void PauseMenu::open()
{
    _isOpen = !_isOpen;
    if (_isOpen)
    {
        _context->getAudio().play("audio/pause_menu_song.wav");
    }
    else
    {
        _context->getAudio().play("audio/back_pocket.wav");
    }
    
}

void PauseMenu::draw()
{
    if (_isOpen)
    {
        _context->getGraphics()->drawBox(11 * 32, 0, 8 * 32, 13 * 32, 48, 72, 203);
    }
}

bool PauseMenu::isOpen() const
{
    return _isOpen;
}