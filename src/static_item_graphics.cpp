#include "static_item_graphics.h"
StaticItemGraphics::StaticItemGraphics
(
    GraphicsContext* context,
    std::function<void (GraphicsContext&, Entity&, const float)> drawer
) : _context(context), _drawer(drawer)
{
}

void StaticItemGraphics::update(Entity& e, const TimeStep timeStep)
{
    _drawer(*_context, e, timeStep.getTimeStep());
}