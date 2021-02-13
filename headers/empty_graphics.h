#ifndef EMPTY_GRAPHICS
#define EMPTY_GRAPHICS
#include "entity.h"
#include "graphics_handler.h"
#include "graphics_context.h"

class EmptyGraphics : public GraphicsHandler
{
    public:
        static EmptyGraphics* shared_instance(GraphicsContext* context)
        {
            static EmptyGraphics eg(context);
            return &eg;
        }
        void update(Entity& e, const TimeStep timeStep)
        {
            _context->drawHitbox(e.pos.x, e.pos.y, e.body.w, e.body.h);
        }
    private:
        EmptyGraphics(GraphicsContext* context) : _context(context)
        {
        }
        GraphicsContext* _context;
};
#endif
