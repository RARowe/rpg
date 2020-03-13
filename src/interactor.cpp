#include "entity.h"
#include "interactor.h"

void Interactor::addEntity(Entity* entity)
{
    _entities.push_back(entity);
}

void Interactor::interact(int x, int y)
{
    //for (auto e : _entities)
    //{
    //    if (e->contains(x, y))
    //    {
    //        e->interact();
    //    }
    //}
}