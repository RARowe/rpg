#ifndef GAME_MATH_H
#define GAME_MATH_H
#include <math.h>
#include "enums.h"
#include "types.h"

static inline float secondPower(float x)
{
    return x * x;
}

float inline position(float velocity, float time, float initialPosition)
{
    return velocity * time + initialPosition;
}

inline bool point_in_body(const Body& b, const Point& p)
{
	return p.x >= b.x &&
        p.x <= b.x + b.w &&
        p.y >= b.y &&
        p.y <= b.y + b.h;
}

//void calculate_cursor(Point& c, const Body* body, const Direction d)
//{
//    switch (d)
//    {
//        case Direction::LEFT:
//            c.x = body->x - 10;
//            c.y = body->y + (body->h / 2);
//            break;
//        case Direction::RIGHT:
//            c.x = body->x + body->w + 10;
//            c.y = body->y + (body->h / 2);
//            break;
//        case Direction::UP:
//            c.x = body->x + (body->w / 2);
//            c.y = body->y - 10;
//            break;
//        case Direction::DOWN:
//            c.x = body->x + (body->w / 2);
//            c.y = body->y + body->h + 10;
//            break;
//    }
//}
#endif
