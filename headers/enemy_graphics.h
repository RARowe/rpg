#ifndef ENEMY_GRAPHICS_H
#define ENEMY_GRAPHICS_H
#include <cmath>
#include "entity.h"
#include "graphics_handler.h"
#include "graphics_context.h"

class EnemyGraphics : public GraphicsHandler
{
    public:
        static EnemyGraphics* shared_instance(GraphicsContext* context)
        {
            static EnemyGraphics eg(context);
            return &eg;
        }
        void update(Entity& e, const float timeStep)
        {
            _time += timeStep;
            if (_time < 2.0f)
            {
                float junk;
                float decimal = std::modf(_time, &junk);
                if (decimal < 0.25f || (decimal > 0.5f && decimal < 0.75f))
                {
                    _context->drawTexture(e, "enemy.png");
                }
                return;
            }
            else
            {
                _context->drawTexture(e, "enemy.png");
            }
            
            _context->drawHitbox(e.getX(), e.getY(), e.getW(), e.getH());
        }
    private:
        EnemyGraphics(GraphicsContext* context) : _context(context) { }
        GraphicsContext* _context;
        float _time = 0.0f;
};
#endif
