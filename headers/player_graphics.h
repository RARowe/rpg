#ifndef PLAYER_GRAPHICS_H
#define PLAYER_GRAPHICS_H 
#include "graphics_handler.h"
#include "graphics_context.h"

class PlayerGraphics : public GraphicsHandler
{
    public:
        PlayerGraphics(GraphicsContext& context);
        ~PlayerGraphics();
        void update(Entity& e, SDL_Renderer* renderer);
    private:
        int _ticks;
        SDL_Texture* _left[3] = { NULL, NULL, NULL };
        SDL_Texture* _right[3] = { NULL, NULL, NULL };
        SDL_Texture* _up[3] = { NULL, NULL, NULL };
        SDL_Texture* _down[3] = { NULL, NULL, NULL };
        SDL_Rect _rect;
};
#endif
