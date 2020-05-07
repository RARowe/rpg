#ifndef GAME_MATH_H
#define GAME_MATH_H
#include <math.h>
#include "entity.h"
#include "enums.h"

static inline float secondPower(float x)
{
    return x * x;
}

inline float distanceBetween(const Entity e1, const Entity e2)
{
    return sqrt
    (
        secondPower(e1.getX() - e2.getX()) +
        secondPower(e1.getY() - e2.getY())
    );
}

inline Direction relativeDirection(const Entity e1, const Entity e2)
{
    const float relativeX = e2.getX() - e1.getX();
    const float relativeY = e2.getY() - e1.getY();

    if (relativeX > 0)
    {
        if (relativeY <= relativeX && relativeY > -relativeX)
        {
            return Direction::RIGHT;
        }
        else if (relativeY > relativeX)
        {
            return Direction::DOWN;
        }
        else
        {
            return Direction::UP;
        }
    }
    else
    {
        if (relativeY <= -relativeX && relativeY > relativeX)
        {
            return Direction::LEFT;
        }
        else if (relativeY > -relativeX)
        {
            return Direction::DOWN;
        }
        else
        {
            return Direction::UP;
        }
    }
}
#endif