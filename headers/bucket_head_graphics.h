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
        GraphicsContext* _context;
        const Entity* _player;
};
#endif
