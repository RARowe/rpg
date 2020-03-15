#ifndef BUCKET_HEAD_GRAPHICS
#define BUCKET_HEAD_GRAPHICS
#include "entity.h"
#include "graphics_handler.h"
#include "graphics_context.h"

class BucketHeadGraphics : public GraphicsHandler
{
    public:
        BucketHeadGraphics(GraphicsContext& context);
        ~BucketHeadGraphics();
        void update(Entity& e, SDL_Renderer* renderer);
    private:
        int _w;
        int _h;
        SDL_Texture* _sprites;
        SDL_Rect _srcRect;
        SDL_Rect _outRect = { 300, 5, 26, 34 };
};
#endif
