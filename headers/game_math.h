#ifndef GAME_MATH_H
#define GAME_MATH_H
#include <math.h>
#include "enums.h"
#include "types.h"

static inline int squared(int x)
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

inline bool point_in_body(const Body& b, int x, int y)
{
	return x >= b.x &&
        x <= b.x + b.w &&
        y >= b.y &&
        y <= b.y + b.h;
}

inline int distance(int x1, int y1, int x2, int y2) {
    return sqrt(squared(x2 - x1) + squared(y2 -y1));
}
#endif
