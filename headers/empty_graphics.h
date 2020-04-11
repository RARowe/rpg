#ifndef EMPTY_GRAPHICS
#define EMPTY_GRAPHICS
#include <iostream>
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
        void update(Entity& e, const float timeStep)
        {
            _context->drawHitbox(e.getX(), e.getY(), e.getW(), e.getH());
        }
    private:
        EmptyGraphics(GraphicsContext* context) : _context(context)
        {
            std::cout << "how many times is this called" << std::endl;
        }
        GraphicsContext* _context;
};
#endif
