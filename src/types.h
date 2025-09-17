#ifndef TYPES_H
#define TYPES_H

#define SCREEN_WIDTH 608
#define SCREEN_HEIGHT 416
#define CELL 32
#define ROWS 13
#define COLS 19

typedef enum { GTEXT, GTILE, GTILES, GBOX, GOUTLINE, GTILEPICKER } GType;

#define TEXT_SIZE 128
typedef union {
    struct {
        int x, y, w, h;
        char text[TEXT_SIZE];
    } GText;
    struct {
        int x, y, w, h, tile;
    } GTile;
    struct {
        int r, c;
        int *tiles;
    } GTiles;
    struct {
        int x, y, w, h;
        unsigned char r, g, b;
    } GBox;
    struct {
        int tile;
        int x, y, w, h;
        unsigned char r, g, b;
    } GTilePicker;
} GAction;

typedef struct {
    GType type;
    GAction action;
} GInstruction;

#define BUFFER_SIZE 1024
typedef struct {
    int idx;
    GInstruction instructions[BUFFER_SIZE];
} GBuffer;

typedef enum { IUP, IDOWN, IMOUSEMOTION, IMOUSEDOWN } IType;

typedef enum { IFORWARD, IBACK, ILEFT, IRIGHT, IACTION, IEXIT } IAction;

typedef struct {
    IType type;
    IAction action;
    int x, y;
} IInstruction;

typedef struct {
    int idx;
    IInstruction instructions[BUFFER_SIZE];
} IBuffer;

typedef struct {
    int size, head;
    void *m;
} MBuffer;

void game_run(GBuffer *, IBuffer *, MBuffer *m, float ts);

#endif
