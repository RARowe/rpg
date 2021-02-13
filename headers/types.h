#ifndef TYPES_H
#define TYPES_H
#include "entity.h"
#include <string>
#include <vector>

typedef struct Point
{
    float x, y;
} Point;

typedef struct Velocity
{
    float xVel, yVel;
} Velocity;

typedef struct Body {
    unsigned short h, w;
} Body;

typedef struct Speech
{
    std::string speaker;
    std::vector<std::string> lines;
} Speech;

typedef struct Entities {
    unsigned short size;
    unsigned short back;
    Entity* entities;
} Entities;
#endif
