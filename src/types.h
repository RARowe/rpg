#define SCREEN_HEIGHT 416
#define SCREEN_WIDTH 608

// Audio
typedef struct {
    int size;
} SoundQueue;

typedef struct Audio {
    Mix_Music* music;
    Mix_Chunk* sounds;
    bool stopRequested;
    SoundQueue queue;
} Audio;

// Graphics
typedef enum {
    COLOR_WHITE,
    COLOR_BLUE,
    COLOR_BLACK,
    COLOR_RED
} Color;

typedef struct {
    SDL_Texture* texture;
    unsigned int w, h;
    char name[64];
} Texture;

typedef struct {
    int size;
    Texture textures[32];
} TextureCache;

// Input
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
/* SDL Graphics Platform Implementation */
typedef struct Graphics {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    const char* resourceFolderPath;
    TextureCache textureCache;
} Graphics;


typedef enum {
    GAME_STATE_STARTUP,
    GAME_STATE_NORMAL,
    GAME_STATE_TEXTBOX,
    GAME_STATE_EDITOR,
    GAME_STATE_MODAL,
    GAME_STATE_TILE_PICKER,
    GAME_STATE_TEXT_EDITOR
} GameState;

typedef struct {
    char* outBuffer;
    int cursorPos;
    char buffer[1024];
} TextEditor;

typedef struct {
    float x, y;
} Point;

typedef struct {
    float maxVel;
    float xVel, yVel;
} Velocity;

typedef struct {
    float x, y;
    short w, h;
} Body;

typedef struct TextBox {
    int textureId;
    std::string text;
} TextBox;

typedef struct {
    int tile;
    char description[64];
} Item;

typedef struct SceneData {
    unsigned int tileSet = 0;
    unsigned int nextEntityId = 1;
    char name[128];
    size_t backgroundSize = 247;
    int background[247];
    size_t midgroundSize = 247;
    int midground[247];
    size_t foregroundSize = 247;
    int foreground[247];
    Point spawnPoint;
    Velocity vel;
    std::map<int, Body> bodies;
    std::map<int, int> tileSprites;
    std::map<int, std::string> textInteractions;
    std::map<int, Item> items;
    std::set<int> solidEntities;
} SceneData;

typedef struct {
    Item item;
    char text[1024];
} InteractionData;

typedef struct {
    Body dim;
    Point textStartingPoint;
    char* options[5];
    int numberOfOptions;
    int currentSelection;
    int* result;
    bool hasTitle;
    char title[64];
} Modal;

typedef struct {
    unsigned int id, hTiles, vTiles, totalTiles;
} TilesetMeta;

typedef struct {
    TilesetMeta tilesetMeta;
    int tile;
} TilePicker;

// Editor
typedef enum {
    EDITOR_MODE_NORMAL = 0,
    EDITOR_MODE_EDIT,
    EDITOR_MODE_REQUEST_RESOURCE
} EditorMode;

typedef enum {
    TOOL_SELECT = 0,
    TOOL_WALL,
    TOOL_TEXT_INTERACTION,
    TOOL_SPAWN_POINT,
    TOOL_TILE,
    TOOL_ITEM
} Tool;

typedef enum {
    LAYER_BACKGROUND = 0,
    LAYER_MIDGROUND,
    LAYER_FOREGROUND
} Layer;

typedef enum {
    TOOLBAR_STATE_DEFAULT = 0,
    TOOLBAR_STATE_FILE,
    TOOLBAR_STATE_TOOL,
    TOOLBAR_STATE_DEBUG
} ToolbarState;

typedef struct {
    int tile;
    int x, y;
    Layer layer;
} TileEditor;

typedef struct {
    int size;
    int stack[10];
} state_stack_t;

typedef struct Editor {
    bool isInitialzed;
    EditorMode currentMode;
    state_stack_t mode;
    Body* selectedEntity;
    Tool currentTool;
    /* Cursor */
    int curX, curY;
    int startX, startY;
    int relX, relY;
    bool isDragging;
    /* Toolbar */
    ToolbarState toolBarState;
    /* Debug */
    bool showGrid;
    TileEditor tileEditor;
    /* Text edit buffer */
    int result;
    char textBuffer[1024];
} Editor;

typedef struct GameData {
	Graphics graphics;
	Audio audio;
	Input input;
    TextBox textBox;
    Modal modal;
    TilePicker tilePicker;
    SceneData scene;
    Editor editor;
    TextEditor textEditor;
    InteractionData interactionData;
    float foundItem;
    int* tile;
    float lastTime;
    long frame;
    state_stack_t gameState;
    /* Title screen */
    float flashingButton = 0.0f;
    float fadeIn = 0.0f;
    bool startSelected = false;
    float startSelectedFlash = 0.0f;
    /* End */
    bool showFrameRate = false;
    bool openTextBoxRequested = false;
    bool openModalRequested = false;
    bool openTilePickerRequested = false;
    bool openTextEditorRequested = false;
    bool sceneSaveRequested = false;
    bool sceneLoadRequested = false;
} GameData;

static inline int squared(int x) {
    return x * x;
}

float inline position(float velocity, float time, float initialPosition) {
    return velocity * time + initialPosition;
}

inline bool point_in_body(const Body* b, const Point* p) {
	return p->x >= b->x &&
        p->x <= b->x + b->w &&
        p->y >= b->y &&
        p->y <= b->y + b->h;
}

inline bool point_in_body(const Body& b, int x, int y) {
	return x >= b.x &&
        x <= b.x + b.w &&
        y >= b.y &&
        y <= b.y + b.h;
}

inline bool point_in_body(const Body* b, int x, int y) {
	return x >= b->x &&
        x <= b->x + b->w &&
        y >= b->y &&
        y <= b->y + b->h;
}

inline int distance(int x1, int y1, int x2, int y2) {
    return sqrt(squared(x2 - x1) + squared(y2 -y1));
}

inline int clamp_and_wrap(int i, int low, int high) {
    if (i < low) {
        return high;
    } else if (i > high) {
        return low;
    }
    return i;
}

inline bool utils_entities_collide(const Body* b1, const Body* b2) {
    int x2 = b1->x+ b1->w,
        y2 = b1->y + b1->h,
        b2x2 = b2->x + b2->w,
        b2y2 = b2->y + b2->h;
    bool below = b2->y >= y2,
         above = b2y2 <= b1->y,
         left = b2x2 <= b1->x,
         right = b2->x >= x2;
    return !(below || above || left || right);
}
