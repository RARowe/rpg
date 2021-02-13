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
        secondPower(e1.pos.x - e2.pos.x) +
        secondPower(e1.pos.y - e2.pos.y)
    );
}

inline Direction relativeDirection(const Entity e1, const Entity e2)
{
    const float relativeX = e2.pos.x - e1.pos.x;
    const float relativeY = e2.pos.y - e1.pos.y;

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
