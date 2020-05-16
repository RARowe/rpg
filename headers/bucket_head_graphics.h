#ifndef BUCKET_HEAD_GRAPHICS
#define BUCKET_HEAD_GRAPHICS
#include "entity.h"
#include "graphics_handler.h"
#include "graphics_context.h"

class BucketHeadGraphics : public GraphicsHandler
{
    public:
        static GraphicsHandler* getInstance(GraphicsContext* context, const Entity* player);
        void update(Entity& e, const TimeStep timeStep);
    private:
        BucketHeadGraphics(GraphicsContext* context, const Entity* player);
        ~BucketHeadGraphics();
        GraphicsContext* _context;
        const Entity* _player;
        int _w;
        int _h;
        SDL_Texture* _sprites;
        SDL_Rect _srcRect;
};
#endif
