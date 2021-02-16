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

typedef struct PlayerInput {
    bool up, down, left, right;
    bool upClick, downClick, leftClick, rightClick;
    bool select, back, pause;
    uint8_t debug;
} PlayerInput;

#define DEBUG_FRAME_RATE     1
#define DEBUG_TOGGLE_HIT_BOX 2
#endif
