#include <SDL2/SDL.h>
#include <vector>
#include "entity.h"

#ifndef INTERACTOR_H
#define INTERACTOR_H 
class Interactor
{
    public:
		void addEntity(Entity* entity);
		void interact(int x, int y);
    private:
        std::vector<Entity*> _entities;
};
#endif
