#pragma once
#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdbool.h>

#define SCREEN_HEIGHT 416
#define SCREEN_WIDTH 608

/* Input */
typedef enum {
    KEY_UNKNOWN = 0,

    KEY_RETURN = '\r',
    KEY_ESCAPE = '\033',
    KEY_BACKSPACE = '\b',
    KEY_TAB = '\t',
    KEY_SPACE = ' ',
    KEY_EXCLAIM = '!',
    KEY_QUOTEDBL = '"',
    KEY_HASH = '#',
    KEY_PERCENT = '%',
    KEY_DOLLAR = '$',
    KEY_AMPERSAND = '&',
    KEY_QUOTE = '\'',
    KEY_LEFTPAREN = '(',
    KEY_RIGHTPAREN = ')',
    KEY_ASTERISK = '*',
    KEY_PLUS = '+',
    KEY_COMMA = ',',
    KEY_MINUS = '-',
    KEY_PERIOD = '.',
    KEY_SLASH = '/',
    KEY_0 = '0',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9',
    KEY_COLON = ':',
    KEY_SEMICOLON = ';',
    KEY_LESS = '<',
    KEY_EQUALS = '=',
    KEY_GREATER = '>',
    KEY_QUESTION = '?',
    KEY_AT = '@',

    /* Skip uppercase */
    KEY_LEFTBRACKET = '[',
    KEY_BACKSLASH = '\\',
    KEY_RIGHTBRACKET = ']',
    KEY_CARET = '^',
    KEY_UNDERSCORE = '_',
    KEY_BACKQUOTE = '`',
    KEY_a = 'a',
    KEY_b = 'b',
    KEY_c = 'c',
    KEY_d = 'd',
    KEY_e = 'e',
    KEY_f = 'f',
    KEY_g = 'g',
    KEY_h = 'h',
    KEY_i = 'i',
    KEY_j = 'j',
    KEY_k = 'k',
    KEY_l = 'l',
    KEY_m = 'm',
    KEY_n = 'n',
    KEY_o = 'o',
    KEY_p = 'p',
    KEY_q = 'q',
    KEY_r = 'r',
    KEY_s = 's',
    KEY_t = 't',
    KEY_u = 'u',
    KEY_v = 'v',
    KEY_w = 'w',
    KEY_x = 'x',
    KEY_y = 'y',
    KEY_z = 'z',
} Key;

typedef enum {
    INPUT_STATE_UP = 0,
    INPUT_STATE_PRESSED = 1,
    INPUT_STATE_DOWN = 2,
    INPUT_STATE_RELEASED = 4
} InputState;

typedef enum {
    GAME_INPUT_UP = 0,
    GAME_INPUT_DOWN,
    GAME_INPUT_LEFT,
    GAME_INPUT_RIGHT,
    GAME_INPUT_SELECT,
    GAME_INPUT_BACK,
    GAME_INPUT_ESC,
    GAME_INPUT_CMD,
    GAME_INPUT_CTRL
} GameInput;

typedef struct {
    char game[9];
    char mouseState;
    bool mouseMoving;
    int mouseX, mouseY;
    bool hasLastPressedKey;
    Key lastPressedKey;
    char keys[128];
} Input;

inline int input_is(int src, int mask) { return src & mask; }

inline int input_is_down(const Input *i, GameInput input) {
    return i->game[input] & INPUT_STATE_DOWN;
}

inline int input_is_pressed(const Input *i, GameInput input) {
    return i->game[input] & INPUT_STATE_PRESSED;
}

inline int input_key_is_pressed(const Input *i, Key key) {
    return i->keys[key] & INPUT_STATE_PRESSED;
}

/* Audio */
typedef struct Audio Audio;

void audio_queue_sound(Audio *a, int soundId);
void audio_request_stop_music(Audio *a);

/* Graphics */
typedef enum { COLOR_WHITE, COLOR_BLUE, COLOR_BLACK, COLOR_RED } Color;

typedef struct Graphics Graphics;

void graphics_draw_text(Graphics *g, int x, int y, int fontSize,
                        const char *text);
void graphics_draw_wrapped_text(Graphics *g, int x, int y, int fontSize,
                                int maxWidth, const char *text);
void graphics_draw_menu(Graphics *g, int x, int y, int fontSize, char **options,
                        int n);
void graphics_draw_texture(Graphics *g, int id, int x, int y, int w, int h);
void graphics_draw_tiles(Graphics *g, int id, const int *tiles, int count);
void graphics_draw_tile(Graphics *g, int id, int tile, int x, int y, int w,
                        int h);
void graphics_draw_box(Graphics *g, int x, int y, int w, int h, Color c,
                       int alpha);
void graphics_draw_selection(Graphics *g, int x1, int y1, int x2, int y2);
void graphics_draw_grid_overlay(Graphics *g);
#endif
