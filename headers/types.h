#ifndef TYPES_H
#define TYPES_H
#include <string>
#include <vector>
typedef struct Point
{
    int x, y;
} Point;

typedef struct Speech
{
    std::string speaker;
    std::vector<std::string> lines;
} Speech;
#endif
