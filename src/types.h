#define SCREEN_HEIGHT 416
#define SCREEN_WIDTH 608
//
//// Audio
//typedef struct {
//    int size;
//} SoundQueue;
//
//// Graphics
//typedef enum {
//    COLOR_WHITE,
//    COLOR_BLUE,
//    COLOR_BLACK,
//    COLOR_RED
//} Color;
//
//typedef struct {
//    SDL_Texture* texture;
//    unsigned int w, h;
//    char name[64];
//} Texture;
//
//typedef struct {
//    int size;
//    Texture textures[32];
//} TextureCache;
//
//// Input
//
typedef enum {
	INPUT_STATE_UP = 0,
	INPUT_STATE_PRESSED = 1,
	INPUT_STATE_DOWN = 2,
	INPUT_STATE_RELEASED = 4
} InputState;
//
typedef enum {
	GAME_INPUT_UP = 0,
	GAME_INPUT_DOWN = 1,
	GAME_INPUT_LEFT = 2,
	GAME_INPUT_RIGHT = 4,
	GAME_INPUT_SELECT = 8,
	GAME_INPUT_BACK = 16,
	GAME_INPUT_ESC = 32,
	GAME_INPUT_CMD = 64,
	GAME_INPUT_CTRL = 128
} GameInput;
//
typedef struct {
	unsigned int pressed;
	unsigned int down;
	unsigned int released;
	unsigned int up;
	char mouseState;
	bool mouseMoving;
	int mouseX, mouseY;
	bool hasLastPressedKey;
	char lastPressedKey;
	char keys[128];
} Input;
///* SDL Graphics Platform Implementation */
//
//
//typedef enum {
//    GAME_STATE_STARTUP,
//    GAME_STATE_NORMAL,
//    GAME_STATE_TEXTBOX,
//    GAME_STATE_EDITOR,
//    GAME_STATE_MODAL,
//    GAME_STATE_TILE_PICKER,
//    GAME_STATE_TEXT_EDITOR
//} GameState;
//
//typedef struct {
//    char* outBuffer;
//    int cursorPos;
//    char buffer[1024];
//} TextEditor;
//
//typedef struct {
//    float x, y;
//} Point;
//
//typedef struct {
//    float maxVel;
//    float xVel, yVel;
//} Velocity;
//
//typedef struct {
//    float x, y;
//    short w, h;
//} Body;
//
//typedef struct TextBox {
//    int textureId;
//    char text[512];
//} TextBox;
//
//typedef struct {
//    int tile;
//    char description[64];
//} Item;
//
//// Flags?
//typedef enum {
//	ENTITY_REGION,
//	ENTITY_TYPE_ITEM,
//	ENTITY_TYPE_SOLID,
//		EntityTYpeInteractable
//} EntityType;
//
//typedef struct {
//	unsigned int id;
//	EntityType type;
//	Body body;
//	Velocity vel;
//	int sprite;
//	char text[256];
//	Item item;
//	int isSolid;
//} Entity;
//
//#define MAX_ENTITIES 128
//#define MAX_TILSE 247
//typedef struct SceneData {
//    unsigned int tileSet;
//    unsigned int nextEntityId;
//    char name[128];
//    int background[247];
//    int midground[247];
//    int foreground[247];
//    Point spawnPoint;
//    Velocity vel;
//    Entity entities[MAX_ENTITIES];
//} SceneData;
//
//typedef struct {
//    Item item;
//    char text[1024];
//} InteractionData;
//
//typedef struct {
//    Body dim;
//    Point textStartingPoint;
//    char* options[5];
//    int numberOfOptions;
//    int currentSelection;
//    int* result;
//    bool hasTitle;
//    char title[64];
//} Modal;
//
//typedef struct {
//    unsigned int id, hTiles, vTiles, totalTiles;
//} TilesetMeta;
//
//typedef struct {
//    TilesetMeta tilesetMeta;
//    int tile;
//} TilePicker;
//
//// Editor
//typedef enum {
//    EDITOR_MODE_NORMAL = 0,
//    EDITOR_MODE_EDIT,
//    EDITOR_MODE_REQUEST_RESOURCE
//} EditorMode;
//
//typedef enum {
//    TOOL_SELECT = 0,
//    TOOL_WALL,
//    TOOL_TEXT_INTERACTION,
//    TOOL_SPAWN_POINT,
//    TOOL_TILE,
//    TOOL_ITEM
//} Tool;
//
//typedef enum {
//    LAYER_BACKGROUND = 0,
//    LAYER_MIDGROUND,
//    LAYER_FOREGROUND
//} Layer;
//
//typedef enum {
//    TOOLBAR_STATE_DEFAULT = 0,
//    TOOLBAR_STATE_FILE,
//    TOOLBAR_STATE_TOOL,
//    TOOLBAR_STATE_DEBUG
//} ToolbarState;
//
//typedef struct {
//    int tile;
//    int x, y;
//    Layer layer;
//} TileEditor;
//
//typedef struct {
//    int size;
//    int stack[10];
//} state_stack_t;
//
//typedef struct Editor {
//    bool isInitialzed;
//    EditorMode currentMode;
//    state_stack_t mode;
//    Entity* selectedEntity;
//    Tool currentTool;
//    /* Cursor */
//    int curX, curY;
//    int startX, startY;
//    int relX, relY;
//    bool isDragging;
//    /* Toolbar */
//    ToolbarState toolBarState;
//    /* Debug */
//    bool showGrid;
//    TileEditor tileEditor;
//    /* Text edit buffer */
//    int result;
//    char textBuffer[1024];
//} Editor;
//
typedef struct GameData {
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	Mix_Music* music;
	Mix_Chunk* sounds;
	bool stopRequested;
	Input input;
//    TextBox textBox;
//    Modal modal;
//    TilePicker tilePicker;
//    SceneData scene;
//    Editor editor;
//    TextEditor textEditor;
//    InteractionData interactionData;
//    float foundItem;
//    int* tile;
    float lastTime;
    long frame;
//    state_stack_t gameState;
//    /* Title screen */
//    float flashingButton;
//    float fadeIn;
//    bool startSelected;
//    float startSelectedFlash;
//    /* End */
//    bool showFrameRate;
//    bool openTextBoxRequested;
//    bool openModalRequested;
//    bool openTilePickerRequested;
//    bool openTextEditorRequested;
//    bool sceneSaveRequested;
//    bool sceneLoadRequested;
} GameData;
//
//static inline int squared(int x) {
//    return x * x;
//}
//
//float inline position(float velocity, float time, float initialPosition) {
//    return velocity * time + initialPosition;
//}
//
//
//
//inline bool point_in_body(const Body* b, int x, int y) {
//	return x >= b->x &&
//        x <= b->x + b->w &&
//        y >= b->y &&
//        y <= b->y + b->h;
//}
//
//static inline int distance(int x1, int y1, int x2, int y2) {
//    return sqrt(squared(x2 - x1) + squared(y2 -y1));
//}
//
//inline int clamp_and_wrap(int i, int low, int high) {
//    if (i < low) {
//        return high;
//    } else if (i > high) {
//        return low;
//    }
//    return i;
//}
//
//inline bool utils_entities_collide(const Body* b1, const Body* b2) {
//    int x2 = b1->x+ b1->w,
//        y2 = b1->y + b1->h,
//        b2x2 = b2->x + b2->w,
//        b2y2 = b2->y + b2->h;
//    bool below = b2->y >= y2,
//         above = b2y2 <= b1->y,
//         left = b2x2 <= b1->x,
//         right = b2->x >= x2;
//    return !(below || above || left || right);
//}
