#ifndef GRAPHICS_H
#define GRAPHICS_H 
typedef enum {
    COLOR_WHITE,
    COLOR_BLUE,
    COLOR_BLACK,
    COLOR_RED
} Color;

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

typedef struct {
	void (*draw_box)(int x, int y, int w, int h, Color c, int alpha);
	void (*draw_grid_overlay)();
	void (*draw_menu)(int x, int y, int fontSize, char** options, int n);
	void (*draw_selection)(int x1, int y1, int x2, int y2);
	void (*draw_text)(int x, int y, int w, int h, const char* text);
	void (*draw_text_font)(int x, int y, int fontSize, const char* text);
	void (*draw_texture)(int id, int x, int y, int w, int h);
	void (*draw_tiles)(int id, const int* tiles, int count);
	void (*draw_tile)(int id, int tile, int x, int y, int w, int h);
	void (*draw_wrapped_text)(int x, int y, int fontSize, int maxWidth, const char* text);

	void (*queue_sound)(int soundId);
	void (*request_stop_music)();
} Platform;

inline bool
input_is(int src, int mask) {
    return src & mask;
}

inline bool
input_mouse_is(const Input* i, int mask) {
    return i->mouseState & mask;
}

inline bool
input_is(const Input* i, GameInput input, int mask) {
    return i->game[input] & mask;
}

inline bool
input_is_down(const Input* i, GameInput input) {
    return i->game[input] & INPUT_STATE_DOWN;
}

inline bool
input_is_pressed(const Input* i, GameInput input) {
    return i->game[input] & INPUT_STATE_PRESSED;
}

#endif
