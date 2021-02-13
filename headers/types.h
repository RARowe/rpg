#ifndef TYPES_H
#define TYPES_H
#include <string>
#include <vector>
#include "enums.h"

typedef struct Point {
    float x, y;
} Point;

typedef struct Velocity {
    float xVel, yVel;
} Velocity;

typedef struct Body {
    unsigned short h, w;
} Body;

typedef struct Speech {
    std::string speaker;
    std::vector<std::string> lines;
} Speech;

typedef struct PlayerInput {
    bool up, down, left, right;
    bool a, b;
    bool start;
    uint8_t debugActions;
} PlayerInput;

#define SHOW_FRAME_RATE 1
#define SHOW_HITBOXES   2
#endif
