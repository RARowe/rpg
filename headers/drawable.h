#include <SDL2/SDL.h>

#ifndef DRAWABLE_H
#define DRAWABLE_H 
class Drawable
{
    public:
        virtual ~Drawable() {};
        virtual void draw(SDL_Renderer* renderer) = 0;
};
#endif
