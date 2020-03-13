#ifndef COLLIDABLE_H
#define COLLIDABLE_H
class Collidable
{
    public:
        virtual bool isCollision(int x, int y) = 0;
};
#endif
