#ifndef BUCKET_HEAD_GRAPHICS
#define BUCKET_HEAD_GRAPHICS
#include "entity.h"
#include "graphics_handler.h"
#include "graphics_context.h"

class BucketHeadGraphics : public GraphicsHandler
{
    public:
        BucketHeadGraphics(GraphicsContext* context, const Entity* player);
        ~BucketHeadGraphics();
        void update(Entity& e, const float timeStep);
    private:
        GraphicsContext* _context;
        const Entity* _player;
        int _w;
        int _h;
        SDL_Texture* _sprites;
        SDL_Rect _srcRect;
};
#endif
