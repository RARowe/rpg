#include "static_item_graphics.h"
StaticItemGraphics::StaticItemGraphics
(
    GraphicsContext* context,
    std::string name
) : _context(context), _name(name)
{
}

void StaticItemGraphics::update(Entity& e, const float timeStep)
{
    _context->drawTexture(e, _name);
}