#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include "window.h"
#include "graphics_context.h"
#include "keyboard_handler.h"
#include "enums.h"
#include "entity.h"
#include "interactor.h"
#include "grid.h"
#include "tile.h"
//#include "trash.h"
#include "game_context.h"

int main(int argc, char* args[])
{
    GameContext game;
    GraphicsContext* context = game.getGraphics();
    KeyboardHandler* keyboard = game.getKeyboardHandler();
    Grid grid(*context);
    //game.getCollisionDetector()->addObject(&grid);
    //Player player(&game);
    //PlayerDrawer pd(*context, &player);
    //grid.addEntity(context->getTexture("trash.png"), 20, 4);
	auto player = game.getEntity(EntityType::PLAYER);
	auto trash = game.getEntity(EntityType::TRASH);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    Mix_Music *music = Mix_LoadMUS("audio/back_pocket.wav");
    Mix_PlayMusic(music, 1);
    Mix_VolumeMusic(30);

    SDL_Event windowEvent;
    int ticks = 0;
    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
            {
                break;
            }
            if (windowEvent.type == SDL_KEYDOWN || windowEvent.type == SDL_KEYUP)
            {
                keyboard->handleEvent(windowEvent.key);
            }
        }

        player->processInput(*keyboard);
        player->update();
        grid.draw(context->getRenderer());
        player->draw(context->getRenderer());
		trash->draw(context->getRenderer());
        SDL_RenderPresent(context->getRenderer());

        SDL_Delay(1000 / GraphicsContext::FRAME_RATE);
        ticks++;
    }

    return EXIT_SUCCESS;
}
