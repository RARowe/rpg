#ifndef STATIC_ITEM_GRAPHICS_H
#define STATIC_ITEM_GRAPHICS_H 
#include <SDL2/SDL.h>
#include "entity.h"
#include "enums.h"
#include "graphics_handler.h"

class StaticItemGraphics : public GraphicsHandler
{
    public:
        StaticItemGraphics(SDL_Texture* texture);
        ~StaticItemGraphics();
        void update(Entity& e, SDL_Renderer* renderer);
    private:
		SDL_Texture* _texture;
        SDL_Rect _rect;
};
#endif
