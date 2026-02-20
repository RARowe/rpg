#include <assert.h>
#include <string.h>
#include <strings.h>

#include "types.h"
#define u8 unsigned char
#define u32 unsigned int

/* TODO:
 * - Make script data dynamic
 */

typedef enum {
    SOVERWORLD,
    STEXT,
    SMENU,
    SEDITOR,
    STILEPICKER,
    SSCRIPT,
    SSCRIPTDONE,
    SCUSTOM
} StateType;

typedef struct {
    StateType type;
    void *arg0, *arg1, *arg2;
} State;

typedef struct {
    int size;
    State stack[16];
} StateStack;

static void stack_init(StateStack *s) { s->size = 0; }

static State stack_peek(StateStack *s) {
    assert(s->size > 0);
    return s->stack[s->size - 1];
}

static void stack_push(StateStack *s, StateType state) {
    assert(s->size < 16);
    s->stack[s->size].type = state;
    s->size += 1;
}

static void stack_push_1(StateStack *s, StateType state, void *arg0) {
    assert(s->size < 16);
    s->stack[s->size].type = state;
    s->stack[s->size].arg0 = arg0;
    s->size += 1;
}

/*
static void stack_push_3(StateStack *s, StateType state, void *arg0, void *arg1,
                         void *arg2) {
    assert(s->size < 16);
    s->stack[s->size].type = state;
    s->stack[s->size].arg0 = arg0;
    s->stack[s->size].arg1 = arg1;
    s->stack[s->size].arg2 = arg2;
    s->size += 1;
}
*/

static State stack_pop(StateStack *s) {
    assert(s->size > 0);
    s->size -= 1;
    return s->stack[s->size];
}

static int point_in_body(int x1, int y1, int w1, int h1, int x2, int y2) {
    return x2 >= x1 && x2 <= x1 + w1 && y2 >= y1 && y2 <= y1 + h1;
}
static int is_collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2,
                        int h2) {
    int below, above, left, right;

    below = y2 >= y1 + h1;
    above = y2 + h2 <= y1;
    left = x2 + w2 <= x1;
    right = x2 >= x1 + w1;

    return !(below || above || left || right);
}
static void draw_text(GBuffer *g, int x, int y, int w, int h, const char *text);

static void draw_tiles(GBuffer *g, int r, int c, int *tiles);

static void draw_tile(GBuffer *g, int x, int y, int w, int h, int tile);

static void draw_box(GBuffer *g, int x, int y, int w, int h, u8 red, u8 grn,
                     u8 blu);

static void draw_outline(GBuffer *g, int x, int y, int w, int h, u8 red, u8 grn,
                         u8 blu);
static void draw_tile_picker(GBuffer *g, int tile);

typedef enum { TVIS = 1, TSOLID = 2, TINTERACTABLE = 4 } Trait;
typedef enum { DNORTH = 1, DSOUTH = 2, DEAST = 3, DWEST = 4 } Direction;
typedef enum { ETNPC, ETITEM } EntityType;

typedef struct {
    EntityType type;
    int id, x, y, w, h, tile, xV, yV, dir;
    int traits;
    char text[128];
} Entity;

static int entity_is_solid(Entity *e) { return e->traits & TSOLID; }
static int entity_is_visible(Entity *e) { return e->traits & TVIS; }
static int entity_is_interactable(Entity *e) {
    return e->traits & TINTERACTABLE;
}

#define MAX_INV 32
typedef struct {
    int size;
    int id[MAX_INV];
    int tile[MAX_INV];
    char *desc[MAX_INV];
    int textBufferSize;
    char textBuffer[4096];
} Inventory;

static int inv_put(Inventory *inv, u32 id, u32 tile, char *text) {
    if (inv->size < 32) {
        int srcTextLen;

        inv->id[inv->size] = id;
        inv->tile[inv->size] = tile;

        srcTextLen = strlen(text);
        assert(inv->textBufferSize + srcTextLen < 4096);
        inv->desc[inv->size] =
            strcpy(inv->textBuffer + inv->textBufferSize, text);
        /* Add one for null character */
        inv->textBufferSize = srcTextLen + 1;
        inv->size += 1;

        return 0;
    }

    return -1;
}

static int inv_contains(Inventory *inv, u32 id) {
    int i;
    for (i = 0; i < inv->size; i++) {
        if (inv->id[i] == id) {
            return -1;
        }
    }
    return 0;
}

struct GameState;
typedef struct GameState GameState;

typedef void (*Callback)(GameState *);
typedef void (*CustomScript)(GameState *, float);

typedef struct {
    u32 cursor, size;
    char **options;
    Callback *callbacks;
} SelectMenu;

typedef struct {
    u32 size, cursor;
    State state[16];
} ScriptEngine;

static int script_engine_next(ScriptEngine *se) {
    se->cursor += 1;
    if (se->cursor >= se->size) {
        return 0;
    }
    return -1;
}

static StateType script_engine_current_state_type(ScriptEngine *se) {
    if (se->cursor < se->size) {
        return se->state[se->cursor].type;
    }

    return SSCRIPTDONE;
}

static State script_engine_current_state(ScriptEngine *se) {
    State nullState = {SSCRIPTDONE};
    if (se->cursor < se->size) {
        return se->state[se->cursor];
    }

    return nullState;
}

static void script_engine_reset(ScriptEngine *se) {
    se->size = 0;
    se->cursor = 0;
}

static void script_engine_push_1(ScriptEngine *se, StateType state,
                                 void *arg0) {
    assert(se->size < 16);
    se->state[se->size].type = state;
    se->state[se->size].arg0 = arg0;
    se->size += 1;
}

static void script_engine_push_3(ScriptEngine *se, StateType state, void *arg0,
                                 void *arg1, void *arg2) {
    assert(se->size < 16);
    se->state[se->size].type = state;
    se->state[se->size].arg0 = arg0;
    se->state[se->size].arg1 = arg1;
    se->state[se->size].arg2 = arg2;
    se->size += 1;
}

#define MAX_ENTITIES 16
struct GameState {
    u32 idIncrement;
    int background[247];
    StateStack stack;
    u32 stackPopRequests;
    ScriptEngine scriptEngine;
    int scriptInitialized;

    Entity player;
    int entityCount;
    Entity entities[MAX_ENTITIES];
    Inventory inv;

    /* TODO: Add a memory buffer here for the scripts */
    int edX, edY;
    Entity *edE;
    int selectedTile;
    int tC, tTotal;
    SelectMenu menu;
};

static StateType game_state_current_state_type(GameState *s) {
    if (stack_peek(&s->stack).type == SSCRIPT) {
        return script_engine_current_state_type(&s->scriptEngine);
    }

    return stack_peek(&s->stack).type;
}

static void game_state_request_next_state(GameState *s) {
    if (stack_peek(&s->stack).type == SSCRIPT) {
        if (!script_engine_next(&s->scriptEngine)) {
            /* No more steps left in script, so reset, and pop script state */
            script_engine_reset(&s->scriptEngine);
            s->stackPopRequests += 1;
            s->scriptInitialized = 0;
        }
        return;
    }

    s->stackPopRequests += 1;
}

void script_run(GameState *s, u32 script, float dt);
void script_run(GameState *s, u32 script, float dt);

typedef struct {
    u32 id;
    char *name;
} Script;

static Entity *entity_find_by_point(GameState *s, int x, int y) {
    int i;
    for (i = 0; i < s->entityCount; i++) {
        Entity *e = &s->entities[i];
        if (point_in_body(e->x, e->y, e->w, e->h, x, y)) {
            return e;
        }
    }

    return 0;
}

static void request(GameState *s, Entity *e) {
    switch (e->type) {
    case ETNPC:
        stack_push_1(&s->stack, SSCRIPT, 0);
        break;
    case ETITEM:
        /* Maybe we could run the add inv
         * event at the beginning of frame?
         */
        stack_push_1(&s->stack, STEXT, e->text);
        /* TODO: Do something when inv is full */
        assert(inv_put(&s->inv, 123, e->tile, e->text) == 0);
        e->traits = 0;
        break;
    }
}

static void process_overworld_input(IBuffer *in, GameState *s) {
    int i;
    Entity *p;
    p = &s->player;

    for (i = 0; i < in->idx; i++) {
        if (in->instructions[i].type == IDOWN) {
            switch (in->instructions[i].action) {
            case IFORWARD:
                p->yV = -300.0f;
                p->dir = DNORTH;
                p->tile = 110;
                break;
            case IBACK:
                p->yV = 300.0f;
                p->dir = DSOUTH;
                p->tile = 111;
                break;
            case ILEFT:
                p->xV = -300.0f;
                p->dir = DWEST;
                p->tile = 112;
                break;
            case IRIGHT:
                p->xV = 300.0f;
                p->dir = DEAST;
                p->tile = 113;
                break;
            case IACTION: {
                int j, px, py;
                switch (p->dir) {
                case DNORTH:
                    px = p->x + (p->w / 2);
                    py = p->y - 4;
                    break;
                case DSOUTH:
                    px = p->x + (p->w / 2);
                    py = p->y + p->h + 4;
                    break;
                case DEAST:
                    px = p->x + p->w + 4;
                    py = p->y + (p->h / 2);
                    break;
                default:
                    px = p->x - 4;
                    py = p->y + (p->h / 2);
                    break;
                }
                for (j = 0; j < s->entityCount; j++) {
                    Entity *e = &s->entities[j];

                    if (entity_is_interactable(e) &&
                        point_in_body(e->x, e->y, e->w, e->h, px, py)) {
                        request(s, e);
                    }
                }
                break;
            }
            case IEXIT: {
                stack_push(&s->stack, SEDITOR);
                break;
            }
            default:
                continue;
            }
        } else {
            switch (in->instructions[i].action) {
            case IFORWARD:
                p->yV = 0.0f;
                break;
            case IBACK:
                p->yV = 0.0f;
                break;
            case ILEFT:
                p->xV = 0.0f;
                break;
            case IRIGHT:
                p->xV = 0.0f;
                break;
            default:
                continue;
            }
        }
    }
}

static void process_text_box_input(IBuffer *in, GameState *s) {
    int i;
    for (i = 0; i < in->idx; i++) {
        if (in->instructions[i].type == IDOWN) {
            if (in->instructions[i].action == IACTION) {
                game_state_request_next_state(s);
                return;
            }
        }
    }
}
static void process_menu_input(IBuffer *in, GameState *s) {
    int i, fireAction;
    fireAction = 0;
    for (i = 0; i < in->idx; i++) {
        if (in->instructions[i].type == IDOWN) {
            if (in->instructions[i].action == IFORWARD) {
                s->menu.cursor -= 1;
            }

            if (in->instructions[i].action == IBACK) {
                s->menu.cursor += 1;
            }

            if (in->instructions[i].action == IACTION) {
                fireAction = 1;
            }
        }
    }

    if (s->menu.cursor < 0) {
        s->menu.cursor = s->menu.size - 1;
    } else if (s->menu.cursor >= s->menu.size) {
        s->menu.cursor = 0;
    }

    if (fireAction) {
        s->menu.callbacks[s->menu.cursor](s);
        game_state_request_next_state(s);
    }
}

static void process_editor_input(IBuffer *in, GameState *s) {
    int i;
    for (i = 0; i < in->idx; i++) {
        if (in->instructions[i].type == IDOWN) {
            if (in->instructions[i].action == IEXIT) {
                game_state_request_next_state(s);
                return;
            } else if (in->instructions[i].action == IACTION) {
                stack_push(&s->stack, STILEPICKER);
                return;
            }
        } else if (in->instructions[i].type == IMOUSEMOTION) {
            s->edX = (in->instructions[i].x / 32) * 32;
            s->edY = (in->instructions[i].y / 32) * 32;
        } else if (in->instructions[i].type == IMOUSEDOWN) {
            Entity *e;
            if ((e = entity_find_by_point(s, in->instructions[i].x,
                                          in->instructions[i].y))) {
                s->edE = e;
            }
        }
    }
}
static void process_tile_picker_input(IBuffer *in, GameState *s) {
    int i;
    for (i = 0; i < in->idx; i++) {
        if (in->instructions[i].type == IDOWN) {
            switch (in->instructions[i].action) {
            case IFORWARD:
                s->selectedTile -= s->tC;
                break;
            case IBACK:
                s->selectedTile += s->tC;
                break;
            case ILEFT:
                s->selectedTile--;
                break;
            case IRIGHT:
                s->selectedTile++;
                break;
            case IEXIT:
                game_state_request_next_state(s);
                return;
            default:
                break;
            }
        }
    }

    if (s->selectedTile < 0) {
        s->selectedTile = 0;
    } else if (s->selectedTile >= s->tTotal) {
        s->selectedTile = s->tTotal - 1;
    }
}

static void process_input(IBuffer *in, GameState *s) {
    switch (game_state_current_state_type(s)) {
    case SOVERWORLD:
        process_overworld_input(in, s);
        break;
    case STEXT:
        process_text_box_input(in, s);
        break;
    case SMENU:
        process_menu_input(in, s);
        break;
    case SEDITOR:
        process_editor_input(in, s);
        break;
    case STILEPICKER:
        process_tile_picker_input(in, s);
        break;
    default:
        assert("No state selected");
        break;
    }
}

static void run(GameState *s, float ts) {
    int prevX, prevY;
    Entity *p = &s->player;

    prevX = p->x;
    prevY = p->y;
    p->w = 32;
    p->h = 32;
    p->x += p->xV * ts;
    p->y += p->yV * ts;

    if (p->x < -32) {
        p->x = SCREEN_WIDTH + 32;
    }

    if (p->x > SCREEN_WIDTH + 32) {
        p->x = -32;
    }

    if (p->y < -32) {
        p->y = SCREEN_HEIGHT + 32;
    }

    if (p->y > SCREEN_HEIGHT + 32) {
        p->y = -32;
    }

    /* Check collisions */
    {
        int i;
        for (i = 0; i < s->entityCount; i++) {
            Entity *e = &s->entities[i];

            if (entity_is_solid(e)) {
                if (is_collision(p->x, p->y, p->w, p->h, e->x, e->y, e->w,
                                 e->h)) {
                    p->x = prevX;
                    p->y = prevY;
                }
            }
        }
    }
}

static void *galloc(MBuffer *m, int s) {
    void *retVal;
    assert(m->head + s <= m->size);
    /* GCC doesn't like doing pointer arith with (void*) */
    retVal = (u8 *)m->m + m->head;
    m->head += s;
    return retVal;
}

int state_next_id(GameState *s) {
    int retVal = s->idIncrement;
    s->idIncrement += 1;
    return retVal;
}

void entity_new_npc(GameState *s, int x, int y, int w, int h, int tile,
                    char *text) {
    int i = s->entityCount;
    assert(s->entityCount < MAX_ENTITIES);
    s->entities[i].id = state_next_id(s);
    s->entities[i].type = ETNPC;
    s->entities[i].traits = TSOLID | TVIS | TINTERACTABLE;
    s->entities[i].x = x;
    s->entities[i].y = y;
    s->entities[i].w = w;
    s->entities[i].h = h;
    s->entities[i].tile = tile;
    strcpy(s->entities[i].text, text);
    s->entityCount += 1;
}

void entity_new_item(GameState *s, int x, int y, int w, int h, int tile,
                     char *text) {
    int i = s->entityCount;
    assert(s->entityCount < MAX_ENTITIES);
    s->entities[i].id = state_next_id(s);
    s->entities[i].type = ETITEM;
    s->entities[i].traits = TSOLID | TVIS | TINTERACTABLE;
    s->entities[i].x = x;
    s->entities[i].y = y;
    s->entities[i].w = w;
    s->entities[i].h = h;
    strcpy(s->entities[i].text, text);
    s->entityCount += 1;
}

void game_run(GBuffer *g, IBuffer *in, MBuffer *m, float ts) {
    GameState *s;
    Entity *p;
    State currentState;
    if (!m->head) {
        /* INIT */
        int i;
        s = galloc(m, sizeof(GameState));
        for (i = 0; i < 247; i++) {
            s->background[i] = 300;
        }
        s->idIncrement = 1;
        s->inv.size = 0;
        s->inv.textBufferSize = 0;
        s->selectedTile = 0;
        /* Fix this, hard coded */
        s->tC = 37, s->tTotal = 1036;
        stack_init(&s->stack);
        s->stackPopRequests = 0;
        s->scriptInitialized = 0;
        stack_push(&s->stack, SOVERWORLD);

        entity_new_npc(s, 256, 256, 32, 32, 168, "HELLO");
        entity_new_item(s, 320, 320, 32, 32, 201, "Really cool thingy");
    } else {
        /* GameState should always be first structure in mem. */
        s = m->m;
    }

    currentState = stack_peek(&s->stack);
    if (currentState.type == SSCRIPT) {
        State engineState;
        /* TODO: SCRIPT */
        if (!s->scriptInitialized) {
            script_run(s, (long)currentState.arg0, ts);
            s->scriptInitialized = -1;
        }
        /* If a script is running, the current state may change. */
        engineState = script_engine_current_state(&s->scriptEngine);
        if (engineState.type != SSCRIPTDONE) {
            currentState = engineState;
        }
    }

    /* TODO: We need some init conditions here */
    if (currentState.type == SMENU) {
        s->menu.options = currentState.arg0;
        s->menu.callbacks = currentState.arg1;
        s->menu.size = (long)currentState.arg2;
    }

    if (currentState.type == SCUSTOM) {
        ((CustomScript)currentState.arg0)(s, ts);
    }

    process_input(in, s);
    run(s, ts);

    p = &s->player;

    /* Draw overworld */
    draw_tiles(g, ROWS, COLS, s->background);

    draw_tile(g, p->x, p->y, p->w, p->h, p->tile);

    {
        int i;
        for (i = 0; i < s->entityCount; i++) {
            Entity *e = &s->entities[i];

            if (entity_is_visible(e)) {
                draw_tile(g, e->x, e->y, e->w, e->h, e->tile);
            }
        }
    }

    /* Draw textbox */
    if (currentState.type == STEXT) {
        draw_box(g, 0, 256, SCREEN_WIDTH, 160, 0, 0, 255);
        draw_text(g, 0, 256, SCREEN_WIDTH, 64, (char *)currentState.arg0);
    }

    /* Draw editor */
    if (currentState.type == SEDITOR) {
        if (s->edX < 256 && s->edY < 64) {
            draw_text(g, 352, 0, 256, 32, "EDIT");
        } else {
            draw_text(g, 0, 0, 256, 32, "EDIT");
        }
        draw_outline(g, s->edX, s->edY, CELL, CELL, 200, 0, 0);
        if (s->edE) {
            draw_outline(g, s->edE->x, s->edE->y, CELL, CELL, 200, 0, 0);
        }
    }

    /* Draw tile picker */
    if (currentState.type == STILEPICKER) {
        draw_tile_picker(g, s->selectedTile);
    }

    if (currentState.type == SMENU) {
        int i;
        for (i = 0; i < s->menu.size; i++) {
            draw_text(g, 0, 32 * i, 256, 32, s->menu.options[i]);
            if (i == s->menu.cursor) {
                draw_text(g, 256, 32 * i, 32, 32, "<");
            }
        }
    }

    if (currentState.type == SMENU) {
        int i;
        for (i = 0; i < s->menu.size; i++) {
            draw_text(g, 0, 32 * i, 256, 32, s->menu.options[i]);
            if (i == s->menu.cursor) {
                draw_text(g, 256, 32 * i, 32, 32, "<");
            }
        }
    }

    /* Pop all states of stack requested */
    {
        int i;
        for (i = 0; i < s->stackPopRequests; i++) {
            stack_pop(&s->stack);
        }
        s->stackPopRequests = 0;
    }
}

static void draw_tile_picker(GBuffer *g, int tile) {
    assert(g->idx < BUFFER_SIZE);
    g->instructions[g->idx].type = GTILEPICKER;
    g->instructions[g->idx].action.GTilePicker.tile = tile;
    g->idx += 1;
}

static void draw_text(GBuffer *g, int x, int y, int w, int h,
                      const char *text) {
    assert(g->idx < BUFFER_SIZE);
    g->instructions[g->idx].type = GTEXT;
    g->instructions[g->idx].action.GText.x = x;
    g->instructions[g->idx].action.GText.y = y;
    g->instructions[g->idx].action.GText.w = w;
    g->instructions[g->idx].action.GText.h = h;
    strcpy(g->instructions[g->idx].action.GText.text, text);
    g->idx += 1;
}

static void draw_box(GBuffer *g, int x, int y, int w, int h, u8 red, u8 grn,
                     u8 blu) {
    assert(g->idx < BUFFER_SIZE);
    g->instructions[g->idx].type = GBOX;
    g->instructions[g->idx].action.GBox.x = x;
    g->instructions[g->idx].action.GBox.y = y;
    g->instructions[g->idx].action.GBox.w = w;
    g->instructions[g->idx].action.GBox.h = h;
    g->instructions[g->idx].action.GBox.r = red;
    g->instructions[g->idx].action.GBox.g = grn;
    g->instructions[g->idx].action.GBox.b = blu;
    g->idx += 1;
}

static void draw_outline(GBuffer *g, int x, int y, int w, int h, u8 red, u8 grn,
                         u8 blu) {
    assert(g->idx < BUFFER_SIZE);
    g->instructions[g->idx].type = GOUTLINE;
    g->instructions[g->idx].action.GBox.x = x;
    g->instructions[g->idx].action.GBox.y = y;
    g->instructions[g->idx].action.GBox.w = w;
    g->instructions[g->idx].action.GBox.h = h;
    g->instructions[g->idx].action.GBox.r = red;
    g->instructions[g->idx].action.GBox.g = grn;
    g->instructions[g->idx].action.GBox.b = blu;
    g->idx += 1;
}

static void draw_tiles(GBuffer *g, int r, int c, int *tiles) {
    assert(g->idx < BUFFER_SIZE);
    g->instructions[g->idx].type = GTILES;
    g->instructions[g->idx].action.GTiles.r = r;
    g->instructions[g->idx].action.GTiles.c = c;
    g->instructions[g->idx].action.GTiles.tiles = tiles;
    g->idx += 1;
}

static void draw_tile(GBuffer *g, int x, int y, int w, int h, int tile) {
    assert(g->idx < BUFFER_SIZE);
    g->instructions[g->idx].type = GTILE;
    g->instructions[g->idx].action.GTile.x = x;
    g->instructions[g->idx].action.GTile.y = y;
    g->instructions[g->idx].action.GTile.w = w;
    g->instructions[g->idx].action.GTile.h = h;
    g->instructions[g->idx].action.GTile.tile = tile;
    g->idx += 1;
}

/* sec_script */
typedef enum { SCR_DEFAULT_GRUMPY_MAN } ScriptId;

Script scripts[] = {{SCR_DEFAULT_GRUMPY_MAN, "grumpy_man"}};

void grumpy_man(GameState *s, float dt);

void script_run(GameState *s, u32 script, float dt) {
    switch (script) {
    case SCR_DEFAULT_GRUMPY_MAN:
        grumpy_man(s, dt);
        break;
    default:
        game_state_request_next_state(s);
        break;
    }
}

void move_grumpy_man(GameState *s, float dt) {
    static float totalTime = 0.0f;
    if (totalTime > 1.0f) {
        s->player.yV = 0;
        game_state_request_next_state(s);
        totalTime = 0.0f;
        return;
    }

    s->player.yV = -200;

    totalTime += dt;
}
/* TODO: This needs to be allocated per script */
void grumpy_man_yes(GameState *s) {
    script_engine_push_1(&s->scriptEngine, STEXT,
                         "Fantastic! I think it's somewhere close by...");
    /* Technically the below is ub */
    script_engine_push_1(&s->scriptEngine, SCUSTOM, (void *)move_grumpy_man);
}

void grumpy_man_no(GameState *s) {
    script_engine_push_1(
        &s->scriptEngine, STEXT,
        "Oh... I guess you've got some better things to be doing.");
}

/* We need a better way to allocate these */
char *options[] = {"Yes", "No"};
Callback callbacks[] = {grumpy_man_yes, grumpy_man_no};

void grumpy_man(GameState *s, float dt) {
    if (inv_contains(&s->inv, 123)) {
        static u32 interactions = 0;
        switch (interactions) {
        case 0:
            script_engine_push_1(
                &s->scriptEngine, STEXT,
                "Hey you found it! Can I please have it back?");
            break;
        case 1:
            script_engine_push_1(&s->scriptEngine, STEXT,
                                 "Hey, give that back!");
            break;
        case 2:
            script_engine_push_1(&s->scriptEngine, STEXT,
                                 "Please! I'm begging you!");
            break;
        case 3:
            script_engine_push_1(&s->scriptEngine, STEXT,
                                 "It was a gift from a dear friend!");
            break;
        default:
            script_engine_push_1(&s->scriptEngine, STEXT,
                                 "Okay. Fine. Don't give it back... It "
                                 "wasn't a gift "
                                 "anyway. I was just saying that.");
            break;
        }
        interactions += 1;
    } else {
        /* The current issue is that we keep creating more
         * events on the engine when the script is running.
         * We either want to have some way where these don't
         * get run again, or we pop the script run state from
         * the stack, and let the engine take over
         */
        script_engine_push_1(&s->scriptEngine, STEXT,
                             "I can't find my wallet. Can you help me?");
        script_engine_push_3(&s->scriptEngine, SMENU, options, callbacks,
                             (void *)2);
    }
}

/*
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>


// Get all types first
#define SCREEN_HEIGHT 416
#define SCREEN_WIDTH 608
#define DEBUG puts
//
//// Audio
//typedef struct {
//    int size;
//} SoundQueue;
//
// Graphics
typedef enum {
    COLOR_WHITE,
    COLOR_BLUE,
    COLOR_BLACK,
    COLOR_RED
} Color;
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
        unsigned char keys[128];
} Input;
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
//    int curX, curY;
//    int startX, startY;
//    int relX, relY;
//    bool isDragging;
//    ToolbarState toolBarState;
//    bool showGrid;
//    TileEditor tileEditor;
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
        //    float flashingButton;
        //    float fadeIn;
        //    bool startSelected;
        //    float startSelectedFlash;
        bool showFrameRate;
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

// Platform
//static void
//audio_queue_sound(Audio* a, int soundId) {
//    // TODO: This does nothing interesting
//    a->queue.size += 1;
//}
//
//static void
//audio_request_stop_music(Audio* a) {
//    a->stopRequested = true;
//}
//
//static int
//beginning_audio(Audio* a) {
//
//    // TODO: These should go somewhere else
//    a->music = Mix_LoadMUS("resources/welcome.wav");
//    a->sounds = Mix_LoadWAV("resources/start.ogg");
//    a->stopRequested = false;
//    a->queue.size = 0;
//
//    Mix_PlayMusic(a->music, -1);
//
//    return 0;
//}
//
//static void
//audio_process(Audio* a) {
//    if (a->stopRequested) {
//        Mix_HaltMusic();
//    }
//
//    for (int i = 0; i < a->queue.size; i++) {
//        Mix_PlayChannel(-1, a->sounds, 0);
//    }
//
//    a->queue.size = 0;
//}
//
//static void
//audio_shutdown(Audio* a) {
//    Mix_FreeChunk(a->sounds);
//    Mix_FreeMusic(a->music);
//    Mix_CloseAudio();
//}
//
// Helpers
static inline bool input_is(int src, int mask) {
        return src & mask;
}

bool
input_key_is_pressed(const Input* i, unsigned char key) {
        return i->keys[key] & INPUT_STATE_PRESSED;
}
//
//inline bool
//input_mouse_is(const Input* i, int mask) {
//    return i->mouseState & mask;
//}
//
//inline bool
//input_is_down(const Input* i, GameInput input) {
//    return i->game[input] & INPUT_STATE_DOWN;
//}
//
//inline bool
//input_game_input_is_pressed(const Input* i, GameInput input) {
//    return i->game[input] & INPUT_STATE_PRESSED;
//}
//
//inline bool
//input_key_is_pressed(const Input* i, Key key) {
//    return i->keys[key] & INPUT_STATE_PRESSED;
//}
//
static void input_handle_key_event(Input* i, SDL_Event* event) {
        SDL_Keycode key = event->key.keysym.sym;
        bool keyDown = event->type == SDL_KEYDOWN;

        switch (key) {
                case SDLK_UP:
                        if (keyDown) {
                                i->pressed |= GAME_INPUT_UP;
                                i->down |= GAME_INPUT_UP;
                        } else {
                                i->released |= GAME_INPUT_UP;
                        }
                        break;
                case SDLK_DOWN:
                        if (keyDown) {
                                i->pressed |= GAME_INPUT_DOWN;
                                i->down |= GAME_INPUT_DOWN;
                        } else {
                                i->released |= GAME_INPUT_DOWN;
                        }
                        break;
                case SDLK_LEFT:
                        if (keyDown) {
                                i->pressed |= GAME_INPUT_LEFT;
                                i->down |= GAME_INPUT_LEFT;
                        } else {
                                i->released |= GAME_INPUT_LEFT;
                        }
                        break;
                case SDLK_RIGHT:
                        if (keyDown) {
                                i->pressed |= GAME_INPUT_RIGHT;
                                i->down |= GAME_INPUT_RIGHT;
                        } else {
                                i->released |= GAME_INPUT_RIGHT;
                        }
                        break;
                case SDLK_ESCAPE:
                        if (keyDown) {
                                i->pressed |= GAME_INPUT_ESC;
                                i->down |= GAME_INPUT_ESC;
                        } else {
                                i->released |= GAME_INPUT_ESC;
                        }
                        break;
                case SDLK_RGUI:
                case SDLK_LGUI:
                        if (keyDown) {
                                i->pressed |= GAME_INPUT_ESC;
                                i->down |= GAME_INPUT_ESC;
                        } else {
                                i->released |= GAME_INPUT_ESC;
                        }
                        break;
                case SDLK_RCTRL:
                case SDLK_LCTRL:
                        if (keyDown) {
                                i->pressed |= GAME_INPUT_CTRL;
                                i->down |= GAME_INPUT_CTRL;
                        } else {
                                i->released |= GAME_INPUT_CTRL;
                        }
                        break;
                default:
                        if (key >= '\r' && key <= 'z') {
                                i->keys[(int)key] = 1;
                                i->hasLastPressedKey = true;
                                i->lastPressedKey = key;
                        }
                        //i->game[GAME_INPUT_SELECT] = i->keys[SDLKf];
                        //i->game[GAME_INPUT_BACK] = i->keys[SDLK_d];
                        break;
        }
}

static int input_process(Input* i) {
        SDL_Event event;

        i->mouseMoving = false;
        i->hasLastPressedKey = false;

        i->pressed = 0;
        i->released = 0;

        if (input_is(i->mouseState, INPUT_STATE_PRESSED)) {
                i->mouseState = INPUT_STATE_DOWN;
        } else if (input_is(i->mouseState, INPUT_STATE_RELEASED)) {
                i->mouseState = INPUT_STATE_UP;
        }

        for (int j = 0; j < 128; j++) {
                if (input_is(i->keys[j], INPUT_STATE_PRESSED)) {
                        i->keys[j] = INPUT_STATE_DOWN;
                } else if (input_is(i->keys[j], INPUT_STATE_RELEASED)) {
                        i->keys[j] = INPUT_STATE_UP;
                }
        }

        while (SDL_PollEvent(&event)) {
                switch (event.type) {
                        case SDL_QUIT:
                                return -1;
                        case SDL_KEYDOWN:
                        case SDL_KEYUP:
                                input_handle_key_event(i, &event);
                                break;
                        case SDL_MOUSEBUTTONDOWN:
                                i->mouseState = INPUT_STATE_PRESSED |
INPUT_STATE_DOWN; break; case SDL_MOUSEBUTTONUP: i->mouseState =
INPUT_STATE_RELEASED | INPUT_STATE_UP; break; case SDL_MOUSEMOTION:
                                i->mouseMoving = true;
                                i->mouseX = event.motion.x;
                                i->mouseY = event.motion.y;
                                break;
                }
        }

        return 0;
}
//
//
//#define MAKEUI(a,b,c,d) \
//	((unsigned int) ( \
//		((unsigned int)(a)) << 24 | \
//		((unsigned int)(b)) << 16 | \
//		((unsigned int)(c)) << 8 | \
//		((unsigned int)(d)) ))
//
//static SDL_Texture* font_get_texture(Graphics* g, const char* text);
//static Texture* graphics_get_texture(Graphics* g, int id);
//static void color_get(Color c, uint8_t* r, uint8_t* g, uint8_t* b);
//static void
//graphics_draw_box(Graphics* gr, int x, int y, int w, int h, Color c, int
alpha);
//
//
static void
graphics_draw_text(Game* g, int x, int y, int w, int h, const char* text) {
        SDL_Rect out = {x, y, w, h};
        SDL_Texture* texture = font_get_texture(g, text);
        SDL_RenderCopy(g->renderer, texture, NULL, &out);
        SDL_DestroyTexture(texture);
}
//
//static void
//graphics_draw_text_font(Graphics* g, int x, int y, int fontSize, const
char* text) {
//	const int charWidth = fontSize * 0.6f;
//	size_t textLength = strlen(text);
//
//	SDL_Rect out = { x, y, charWidth * (int)textLength, fontSize };
//	SDL_Texture* texture = font_get_texture(g, text);
//
//	SDL_RenderCopy(g->renderer, texture, NULL, &out);
//	SDL_DestroyTexture(texture);
//}
//
//static void
//graphics_draw_menu(Graphics* g, int x, int y, int fontSize, char**
options, int n) {
//	int maxStringSize = 0;
//	int curStringSize = 0;
//
//	for (int i = 0; i < n; i++) {
//		while (options[i][curStringSize]) {
//			curStringSize++;
//		}
//		curStringSize--;
//
//		if (maxStringSize < curStringSize) {
//			maxStringSize = curStringSize;
//		}
//		curStringSize = 0;
//	}
//
//	int menuWidth = fontSize * 0.7f * maxStringSize;
//	int menuHeight = (fontSize * 1.2f) * n;
//
//	graphics_draw_box(g, x, y, menuWidth, menuHeight, COLOR_BLUE, 255);
//
//	for (int i = 0; i < n; i++) {
//		graphics_draw_text_font(g, x, y, fontSize, options[i]);
//		y += fontSize + 2;
//	}
//}
//
//static void
//graphics_draw_wrapped_text(Graphics* g, int x, int y, int fontSize, int
maxWidth, const char* text) {
//	const int charWidth = fontSize * 0.6f;
//	const int numberOfCharsPerLine = maxWidth / charWidth;
//
//	int textLineNumber = 0;
//	int numberOfCharsToTake = 1;
//	int newStart = 0;
//	for (int i = 0; i < strlen(text); i++) {
//		if (numberOfCharsToTake == numberOfCharsPerLine) {
//			int oldValueInCaseOfWordWithNoBreaks = i;
//			while (text[i] != ' ') {
//				i--;
//				numberOfCharsToTake--;
//
//				if (i < newStart) {
//					i = oldValueInCaseOfWordWithNoBreaks;
//					numberOfCharsToTake =
numberOfCharsPerLine;
//					break;
//				}
//			}
//			graphics_draw_text_font(g, x, y + (32 * textLineNumber),
fontSize, text);
//
//			textLineNumber++;
//			numberOfCharsToTake = 1;
//			newStart = i + 1;
//		}
//		else {
//			numberOfCharsToTake++;
//		}
//	}
//	graphics_draw_text_font(g, x, y + (32 * textLineNumber), fontSize,
text);
//}
//
//static void
//graphics_draw_texture(Graphics* g, int id, int x, int y, int w, int h) {
//	Texture* t = graphics_get_texture(g, id);
//
//	SDL_Rect in = { 0, 0, (int)t->w, (int)t->h };
//	SDL_Rect out = { x, y, w, h };
//
//	SDL_RenderCopy(g->renderer, t->texture, &in, &out);
//}
//
//static void
//graphics_draw_tiles(Graphics* g, int id, const int* tiles, int count) {
//	const int width = 16;
//	const int height = 16;
//	const int columns = 37;
//	SDL_Texture* tileSetTexture = graphics_get_texture(g, id)->texture;
//	int row = 0;
//	int column = 0;
//	SDL_Rect in = { 0, 0, width, height };
//	SDL_Rect out = { 0, 0, 32, 32 };
//	const int pixelXOffset = 17;
//	const int pixelYOffset = 17;
//	int p;
//	for (int i = 0; i < count; i++) {
//		p = tiles[i];
//		in.x = (p % columns) * pixelXOffset;
//		in.y = (p / columns) * pixelYOffset;
//		out.x = column * 32;
//		out.y = row * 32;
//
//		SDL_RenderCopy(g->renderer, tileSetTexture, &in, &out);
//
//		column++;
//		// Hardcoded. Fix this.
//		if (column == 19)
//		{
//			column = 0;
//			row++;
//		}
//	}
//}
//
//static void
//graphics_draw_tile(Graphics* g, int id, int tile, int x, int y, int w, int
h)
{
//	const int columns = 37;
//	const int pixelXOffset = 17;
//	const int pixelYOffset = 17;
//	const int width = 16;
//	const int height = 16;
//	SDL_Texture* texture = graphics_get_texture(g, id)->texture;
//	SDL_Rect in =
//	{
//		(tile % columns) * pixelXOffset,
//		(tile / columns) * pixelYOffset,
//		width,
//		height
//	};
//	SDL_Rect out = { x, y, w, h };
//
//	SDL_RenderCopy(g->renderer, texture, &in, &out);
//}
//
//static void
//graphics_draw_box(Graphics* gr, int x, int y, int w, int h, Color c, int
alpha) {
//	uint8_t r, g, b;
//	color_get(c, &r, &g, &b);
//
//	SDL_SetRenderDrawBlendMode(gr->renderer, SDL_BLENDMODE_BLEND);
//	SDL_SetRenderDrawColor(gr->renderer, r, g, b, alpha);
//	SDL_Rect rectangle = { x, y, w, h };
//	SDL_RenderFillRect(gr->renderer, &rectangle);
//	SDL_SetRenderDrawBlendMode(gr->renderer, SDL_BLENDMODE_NONE);
//}
//
//static void
//graphics_draw_selection(Graphics* gr, int x1, int y1, int x2, int y2) {
//	uint8_t r, g, b;
//	color_get(COLOR_BLUE, &r, &g, &b);
//
//	SDL_SetRenderDrawColor(gr->renderer, r, g, b, 255);
//	SDL_RenderDrawLine(gr->renderer, x1, y1, x2, y1);
//	SDL_RenderDrawLine(gr->renderer, x1, y1, x1, y2);
//	SDL_RenderDrawLine(gr->renderer, x2, y2, x2, y1);
//	SDL_RenderDrawLine(gr->renderer, x2, y2, x1, y2);
//
//	int x = x1 <= x2 ? x1 : x2;
//	int y = y1 <= y2 ? y1 : y2;
//	int xp = x1 > x2 ? x1 : x2;
//	int yp = y1 > y2 ? y1 : y2;
//	int w = xp - x;
//	int h = yp - y;
//
//	graphics_draw_box(gr, SCREEN_WIDTH, y, w, h, COLOR_BLUE, 100);
//}
//
//static void
//graphics_draw_grid_overlay(Graphics* g) {
//	SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
//
//	for (int i = 31; i < SCREEN_WIDTH; i += 32) {
//		SDL_RenderDrawLine(g->renderer, i, 0, i, SCREEN_HEIGHT);
//	}
//
//	for (int i = 31; i < SCREEN_HEIGHT; i += 32) {
//		SDL_RenderDrawLine(g->renderer, 0, i, SCREEN_WIDTH, i);
//	}
//}
//
//static int
//graphics_get_number_of_textures(Graphics* g) {
//	return g->textureCache.size;
//}
//
//static int
//graphics_init(Graphics* g, const char* title, int w, int h, const char*
resourceFolderPath) {
//	g->window = SDL_CreateWindow(
//			title,
//			SDL_WINDOWPOS_UNDEFINED,
//			SDL_WINDOWPOS_UNDEFINED,
//			w,
//			h,
//			SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
//	if (g->window == NULL) {
//		printf("Could not create window! SDL Error: %s\n",
SDL_GetError());
//		return 0;
//	}
//
//	g->renderer = SDL_CreateRenderer(
//			g->window,
//			-1,
//			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//	if (g->renderer == NULL) {
//		printf("Could not create renderer! SDL Error: %s\n",
SDL_GetError());
//		return 0;
//	}
//
//	if (TTF_Init() < 0) {
//		printf("SDL TTF could not be initialized! SDL Error: %s\n",
SDL_GetError());
//		return 0;
//	}
//
//	char buffer[256];
//	strcpy(buffer, resourceFolderPath);
//	strcat(buffer, "slkscr.ttf");
//	g->font = TTF_OpenFont(buffer, 16);
//	g->resourceFolderPath = resourceFolderPath;
//	{
//		const char* path = "resources/textures";
//		int textureId;
//		char tBuffer[256];
//		Texture t;
//		DIR *d;
//		struct dirent *dir;
//		d = opendir(path);
//		while ((dir = readdir(d)) != NULL) {
//			const char* name = dir->d_name;
//			size_t length = strlen(name);
//			if (name[length - 1] == 'g') {
//				strcpy(tBuffer, path);
//				strcat(tBuffer, "/");
//				strcat(tBuffer, dir->d_name);
//
//				sscanf(dir->d_name, "%d.png", &textureId);
//
//				SDL_Surface* surface = IMG_Load(tBuffer);
//				t.texture =
SDL_CreateTextureFromSurface(g->renderer, surface);
//				SDL_FreeSurface(surface);
//
//				strcpy(t.name, dir->d_name);
//				{
//					unsigned char b[4];
//					FILE* f = fopen(path, "r");
//					fseek(f, 16, SEEK_SET);
//
//					fread(b, sizeof(unsigned char), 4, f);
//					t.w = MAKEUI(b[0], b[1], b[2], b[3]);
//					fread(b, sizeof(unsigned char), 4, f);
//					t.h = MAKEUI(b[0], b[1], b[2], b[3]);
//
//					fclose(f);
//				}
//
//				memcpy(&g->textureCache.textures[textureId], &t,
sizeof(Texture));
//				g->textureCache.size += 1;
//			}
//		}
//
//		closedir(d);
//	}
//
//	return 1;
//}
//
//static void
//graphics_shutdown(Graphics* g) {
//	TTF_CloseFont(g->font);
//	TTF_Quit();
//	SDL_DestroyRenderer(g->renderer);
//	SDL_DestroyWindow(g->window);
//	for (int i = 0; i < g->textureCache.size; i++) {
//		SDL_DestroyTexture(g->textureCache.textures[i].texture);
//	}
//	SDL_Quit();
//}
//
//static void
//graphics_present(Graphics* g) {
//	SDL_RenderPresent(g->renderer);
//}
//
//static Texture*
//graphics_get_texture(Graphics* g, int id) {
//	return &g->textureCache.textures[id];
//}
//
static SDL_Texture*
font_get_texture(Graphics* gr, const char* text) {
        uint8_t r, g, b;
        color_get(COLOR_WHITE, &r, &g, &b);

        SDL_Color color = { r, g, b, 255 };
        SDL_Surface * surface = TTF_RenderText_Solid(gr->font, text, color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(gr->renderer,
surface); SDL_FreeSurface(surface);

        return texture;
}
//
static void
color_get(Color c, uint8_t* r, uint8_t* g, uint8_t* b) {
        switch (c) {
                case COLOR_WHITE:
                        *r = 255;
                        *g = 255;
                        *b = 255;
                        break;
                case COLOR_BLUE:
                        *r = 48;
                        *g = 72;
                        *b = 203;
                        break;
                case COLOR_RED:
                        *r = 255;
                        *g = 0;
                        *b = 0;
                        break;
                case COLOR_BLACK:
                default:
                        *r = 0;
                        *g = 0;
                        *b = 0;
                        break;
        }
}


// Needed
//static void
//game_request_open_text_box(GameData* d, unsigned int textureId, const
char* text) {
//    d->openTextBoxRequested = true;
//    d->textBox.textureId = textureId;
//    strcpy(d->textBox.text, text);
//}
//
//static void
//game_request_open_modal(GameData* d, const Body* size, const Point*
textStartingPoint, const char* title, char** options, int numberOfOptions,
int* result) {
//    d->openModalRequested = true;
//
//    for (int i = 0; i < numberOfOptions; i++) {
//        d->modal.options[i] = options[i];
//    }
//
//    memcpy(&d->modal.dim, size, sizeof(Body));
//    memcpy(&d->modal.textStartingPoint, textStartingPoint, sizeof(Body));
//
//    d->modal.hasTitle = true;
//    strcpy(d->modal.title, title);
//
//    d->modal.numberOfOptions = numberOfOptions;
//    d->modal.result = result;
//}
//
//static void
//game_request_open_tile_picker(GameData* d, int* tile) {
//    d->openTilePickerRequested = true;
//    d->tile = tile;
//}
//
//static void
//game_request_open_text_editor(GameData* d, char* buffer) {
//    d->openTextEditorRequested = true;
//    d->textEditor.outBuffer = buffer;
//
//    size_t buflen = strlen(buffer);
//    if (buflen > 0) {
//        d->textEditor.cursorPos = buflen;
//        strcpy(d->textEditor.buffer, buffer);
//    }
//}
//
//static void
//game_request_scene_save(GameData* d) {
//    d->sceneSaveRequested = true;
//}
//
//static void
//game_request_scene_load(GameData* d) {
//    d->sceneLoadRequested = true;
//}
//
//// Game Code
//
// static Entity* entities_find_next(SceneData* s) {
//     for (int i = 0; i < MAX_ENTITIES; i++) {
// 	    if (s->entities[i].id < 0) {
// 		    s->entities[i].id = s->nextEntityId;
// 		    s->nextEntityId += 1;
// 		    return &(s->entities[i]);
// 	    }
//     }
//
//     perror("NO ENTITIES AVAILABLE. INCREASE LIMIT");
//
//     return NULL;
// }
//
// static Entity* entities_get(SceneData* s, unsigned int id) {
//     for (int i = 0; i < MAX_ENTITIES; i++) {
// 	    if (s->entities[i].id == id) {
// 		    return &(s->entities[i]);
// 	    }
//     }
//
//     char errorMessage[512];
//     sprintf(errorMessage, "COULD NOT FIND ENTITY WITH ID %d. GAME STATE
CORRUPTED", id);
//
//     perror(errorMessage);
//     return NULL;
// }
//
// static Body* entities_get_body(SceneData* s, int entityId) {
// 	return &(entities_get(s, entityId)->body);
// }
//
// static Entity* entities_get_by_point(SceneData* s, float x, float y) {
// 	for (int i = 0; i < MAX_ENTITIES; i++) {
// 		if (s->entities[i].id < 0) {
// 			continue;
// 	    	}
// 		Body* b = &(s->entities[i].body);
// 		if (point_in_body(b, x, y)) {
// 			return &(s->entities[i]);
// 		}
//     }
// 	return NULL;
// }
//
// static void entities_wall_add(SceneData* s, float x, float y, short w,
short h) {
// 	Entity* e = entities_find_next(s);
// 	e->body = { x, y, w, h };
// 	e->isSolid = true;
// }
//
// static void entities_wall_remove(SceneData* s, unsigned int id) {
// 	Entity* e = entities_get(s, id);
// 	e->id = -1;
// }
//
//
// static void entities_item_add
// (SceneData* s, unsigned int id, int tile, const char* description) {
// 	Entity* e = entities_get(s, id);
// 	e->type = ENTITY_TYPE_ITEM;
// 	e->sprite = tile;
// 	strcpy(e->text, description);
// }
//
// static Entity* entities_item_get(SceneData* s, unsigned int id) {
// 	Entity* e = entities_get(s, id);
// 	if (e->type != ENTITY_TYPE_ITEM) {
// 		return NULL;
// 	}
//
//         return e;
// }
//
// static void entities_text_interaction_add(SceneData* s, unsigned int id,
const char* text) {
// 	Entity* e = entities_get(s, id);
// 	e->type = EntityTYpeInteractable;
// 	strcpy(e->text, text);
// }
//
// static size_t
// entities_text_interaction_get(SceneData* s, unsigned int id, char*
buffer) {
// 	Entity* e = entities_get(s, id);
//         strcpy(buffer, e->text);
// 	return 1;
// }
//
// static void entities_spawn_point_set(SceneData* s, float x, float y) {
//     s->spawnPoint.x = x;
//     s->spawnPoint.y = y;
// }
//
//static int modal_handle_input(const Input* i, Modal* m) {
//    if (input_is_pressed(i, GAME_INPUT_DOWN)) {
//        m->currentSelection++;
//    } else if (input_is_pressed(i, GAME_INPUT_UP)) {
//        m->currentSelection--;
//    } else if (input_is_pressed(i, GAME_INPUT_SELECT)) {
//        *(m->result) = m->currentSelection;
//        return 1;
//    } else if (input_is_pressed(i, GAME_INPUT_BACK)) {
//        *(m->result) = -1;
//        return 1;
//    }
//
//    if (m->currentSelection < 0) {
//        m->currentSelection = m->numberOfOptions - 1;
//    } else if (m->currentSelection >= m->numberOfOptions) {
//        m->currentSelection = 0;
//    }
//
//    return 0;
//}
//
//static void modal_draw(Graphics* g, const Modal* m, float timeStep) {
//    graphics_draw_box(g, m->dim.x, m->dim.y, m->dim.w, m->dim.h,
COLOR_BLUE, 255);
//
//    const Point* p = &m->textStartingPoint;
//    graphics_draw_text_font(g, p->x - 32, p->y + (32 *
m->currentSelection), 32, ">");
//
//    for (int i = 0; i < m->numberOfOptions; i++) {
//        graphics_draw_text_font(g, p->x, p->y + (32 * i), 32,
m->options[i]);
//    }
//
//    if (m->hasTitle) {
//        graphics_draw_text_font(g, 128, 32, 64, m->title);
//    }
//}
//
//static int tile_picker_handle_input(const Input* i, TilePicker* p) {
//    if (input_is_pressed(i, GAME_INPUT_UP)) {
//        p->tile -= p->tilesetMeta.hTiles;
//    } else if (input_is_pressed(i, GAME_INPUT_DOWN)) {
//        p->tile += p->tilesetMeta.hTiles;
//    } else if (input_is_pressed(i, GAME_INPUT_LEFT)) {
//        p->tile--;
//    } else if (input_is_pressed(i, GAME_INPUT_RIGHT)) {
//        p->tile++;
//    }
//
//    if (p->tile < 0) {
//        p->tile = 0;
//    } else if (p->tile > p->tilesetMeta.totalTiles) {
//        p->tile = p->tilesetMeta.totalTiles - 1;
//    }
//
//    if (input_is_pressed(i, KEY_f)) {
//        return 1;
//    } else if (input_is_pressed(i, KEY_d)) {
//        p->tile = -1;
//        return 1;
//    }
//
//    return 0;
//}
//
//static int text_editor_handle_input(TextEditor* t, const Input* i) {
//    if (i->hasLastPressedKey) {
//        char ch = i->lastPressedKey;
//        if (ch == '\r') {
//            strcpy(t->outBuffer, t->buffer);
//            bzero(t->buffer, sizeof(char) * 1024);
//            t->cursorPos = 0;
//
//            return 1;
//        } else if (ch == '\b') {
//            t->buffer[t->cursorPos] = '\0';
//            t->cursorPos -= 1;
//            t->cursorPos = t->cursorPos < 0 ? 0 : t->cursorPos;
//
//            return 0;
//        } else {
//            t->buffer[t->cursorPos] = ch;
//            t->cursorPos += 1;
//
//            return 0;
//        }
//    }
//
//    return 0;
//}
//
//static void tile_picker_draw(Graphics* g, const TilePicker* p) {
//    //int textureId = p->tilesetMeta.id;
//    //int tile = p->tile;
//    //int numberOfHorizontalTiles = p->tilesetMeta.hTiles;
//
//    //int col = tile % numberOfHorizontalTiles;
//    //int row = tile / numberOfHorizontalTiles;
//
//    //int maxFrameTilesHorizontal = ((width - 32) / 34) + 1;
//    //int maxFrameTilesVertical = ((height - 32) / 34) + 1;
//    //int frameXOffset = col - (maxFrameTilesHorizontal - 1) < 0 ?
//    //    0 :
//    //    col - (maxFrameTilesHorizontal - 1);
//    //int frameYOffset = row - (maxFrameTilesVertical - 1) < 0 ?
//    //    0 :
//    //    row - (maxFrameTilesVertical - 1);
//
//    //Texture& t = textureCache[textureId];
//    //SDL_Rect in = {
//    //    17 * frameXOffset,
//    //    17 * frameYOffset,
//    //    maxFrameTilesHorizontal * 17,
//    //    maxFrameTilesVertical * 17
//    //};
//    //SDL_Rect out = { 0, 0, width - 1, height - 1 };
//    //SDL_RenderCopy(_renderer, t.texture, &in, &out);
//
//    //float w = ((float)width / (float)maxFrameTilesHorizontal);
//    //float h = ((float)height / (float)maxFrameTilesVertical);
//    //int x = (int) ((float)(maxFrameTilesHorizontal <= col ?
maxFrameTilesHorizontal - 1: col) * w);
//    //int y = (int) ((float)(maxFrameTilesVertical <= row ?
maxFrameTilesVertical - 1 : row) * h);
//    //drawBox(x, y, (int)w, (int)h, Color::WHITE, 100);
//}
//
//void scene_process_interaction(GameData* d, SceneData* s, const Input* i)
{
//	if (!input_is_pressed(i, GAME_INPUT_SELECT)) { return; }
//	Entity* player = entities_get(s, 0);
//	Entity interaction;
//
//	if (entities_get_by_point(s, player->body.x, player->body.y - 5)) {
//		if (entities_text_interaction_get(s, b,
d->interactionData.text)) {
//			game_request_open_text_box(d, 2,
d->interactionData.text);
//		} else if (entities_item_get(s, b, &d->interactionData.item)) {
//			d->foundItem = 5.0f;
//		}
//	}
//}
//
//void scene_save(const SceneData* s) {
//	// TODO: Non-C++ way
//	//char path[256];
//	//strcat(path, "resources/");
//	//strcat(path, s->name);
//
//	//FILE* f = fopen(path, "w");
//
//	//fprintf(f, "next_id=%d\n", s->nextEntityId);
//
//	//// Write tiles
//	//for (int i = 0; i < 247; i++) {
//	//	fprintf(f, "%d,", s->background[i]);
//	//}
//	//fputs("\n", f);
//	//for (int i = 0; i < 247; i++) {
//	//	fprintf(f, "%d,", s->midground[i]);
//	//}
//	//fputs("\n", f);
//	//for (int i = 0; i < 247; i++) {
//	//	fprintf(f, "%d,", s->foreground[i]);
//	//}
//	//fputs("\n", f);
//
//	//fprintf(f, "%f,%f\n", s->spawnPoint.x, s->spawnPoint.y);
//
//	//// Write Bodies
//	//fprintf(f, "%lu ", s->bodies.size() - 1);
//	//for (auto&& pair : s->bodies) {
//	//	int id = pair.first;
//	//	Body b = pair.second;
//	//	if (id == 0) { continue; }
//	//	fprintf(f, "%d:%f,%f,%hd,%hd$", id, b.x, b.y, b.w, b.h);
//	//}
//	//fputs("\n", f);
//
//	//fprintf(f, "%lu ", s->solidEntities.size());
//	//for (int id : s->solidEntities) {
//	//	fprintf(f, "%d,", id);
//	//}
//	//fputs("\n", f);
//
//	//fprintf(f, "%lu ", s->textInteractions.size());
//	//for (auto&& pair : s->textInteractions) {
//	//	int id = pair.first;
//	//	std::string s = pair.second;
//	//	if (id == 0) { continue; }
//	//	fprintf(f, "%d:%s$$", id, s.c_str());
//	//}
//	//fputs("\n", f);
//
//	//fclose(f);
//}
//
//void scene_load(SceneData* s) {
//	FILE* f = fopen("resources/test.level", "r");
//
//	fscanf(f, "next_id=%d\n", &(s->nextEntityId));
//
//
//	for (int i = 0; i < 247; i++) {
//		fscanf(f, "%d,", &(s->background[i]));
//	}
//	// Skip newline
//	fseek(f, 1L, SEEK_CUR);
//	for (int i = 0; i < 247; i++) {
//		fscanf(f, "%d,", &(s->midground[i]));
//	}
//	fseek(f, 1L, SEEK_CUR);
//	for (int i = 0; i < 247; i++) {
//		fscanf(f, "%d,", &(s->foreground[i]));
//	}
//	fseek(f, 1L, SEEK_CUR);
//
//	fscanf(f, "%f,%f", &s->spawnPoint.x, &s->spawnPoint.y);
//	fseek(f, 1L, SEEK_CUR);
//
//	// Read bodies
//	int numberOfBodies = 0;
//	fscanf(f, "%d ", &numberOfBodies);
//	int id;
//	Body b;
//	for (int i = 0; i < numberOfBodies; i++) {
//		fscanf(f, "%d:%f,%f,%hd,%hd$", &id, &b.x, &b.y, &b.w, &b.h);
//		s->bodies[id] = b;
//	}
//	fseek(f, 1L, SEEK_CUR);
//
//	int numberOfSolidEntities = 0;
//	fscanf(f, "%d ", &numberOfSolidEntities);
//	for (int i = 0; i < numberOfSolidEntities; i++) {
//		fscanf(f, "%d,", &id);
//		s->solidEntities.insert(id);
//	}
//	fseek(f, 1L, SEEK_CUR);
//
//	int numberOfTextInteractions = 0;
//	char textBuffer[1024];
//	fscanf(f, "%d ", &numberOfTextInteractions);
//	for (int i = 0; i < numberOfTextInteractions; i++) {
//		fscanf(f, "%d:%[^$\n]$$", &id, textBuffer);
//		s->textInteractions[id] = textBuffer;
//	}
//	fseek(f, 1L, SEEK_CUR);
//
//	fclose(f);
//}
//
//
//char* fileMenu[] = {
//	"Save",
//	"Load Scene",
//	"Open Texture"
//};
//
//char* toolMenu[] = {
//	"Select Tool",
//	"Wall Tool",
//	"Text Interaction",
//	"Spawn Point",
//	"Tile Tool",
//	"Item Tool"
//};
//
//char* debugMenu[] = {
//	"Grid Toggle"
//};
//
//char* numberOfSpeakers[] = {
//	"Two",
//	"Three",
//	"Four"
//};
//
//char* speakers[] = {
//	"Speaker One",
//	"Speaker Two",
//	"Speaker Three",
//	"Speaker Four"
//};
//
//static void
//editor_handle_input_normal(Editor* e, GameData* d, Graphics* g, Input* i,
SceneData* s) {
//	if (input_is(i->mouseState, INPUT_STATE_DOWN)) {
//		state_stack_push(&e->mode, EDITOR_MODE_EDIT);
//		e->currentTool = TOOL_SELECT;
//	}
//
//	if (input_is_pressed(i, KEY_f)) {
//		e->toolBarState = e->toolBarState == TOOLBAR_STATE_FILE ?
//			TOOLBAR_STATE_DEFAULT : TOOLBAR_STATE_FILE;
//	} else if (input_is_pressed(i, KEY_t)) {
//		e->toolBarState = e->toolBarState == TOOLBAR_STATE_TOOL ?
//			TOOLBAR_STATE_DEFAULT : TOOLBAR_STATE_TOOL;
//	} else if (input_is_pressed(i, KEY_d)) {
//		e->toolBarState = e->toolBarState == TOOLBAR_STATE_DEBUG ?
//			TOOLBAR_STATE_DEFAULT : TOOLBAR_STATE_DEBUG;
//	}
//
//	switch (e->toolBarState) {
//		case TOOLBAR_STATE_FILE:
//			if (input_is_pressed(i, KEY_s)) {
//				strcpy(s->name, "test.level");
//				game_request_scene_save(d);
//				e->toolBarState = TOOLBAR_STATE_DEFAULT;
//			}
//
//			if (input_is_pressed(i, KEY_l)) {
//				strcpy(s->name, "test.level");
//				game_request_scene_load(d);
//				e->toolBarState = TOOLBAR_STATE_DEFAULT;
//			}
//			break;
//		case TOOLBAR_STATE_TOOL:
//			if (input_is_pressed(i, KEY_s)) {
//				e->currentTool = TOOL_SELECT;
//				e->toolBarState = TOOLBAR_STATE_DEFAULT;
//				state_stack_push(&e->mode, EDITOR_MODE_EDIT);
//			}
//
//			if (input_is_pressed(i, KEY_w)) {
//				e->currentTool = TOOL_WALL;
//				e->toolBarState = TOOLBAR_STATE_DEFAULT;
//				state_stack_push(&e->mode, EDITOR_MODE_EDIT);
//			}
//
//			if (input_is_pressed(i, KEY_e)) {
//				e->currentTool = TOOL_TEXT_INTERACTION;
//				e->toolBarState = TOOLBAR_STATE_DEFAULT;
//				state_stack_push(&e->mode, EDITOR_MODE_EDIT);
//			}
//
//			if (input_is_pressed(i, KEY_p)) {
//				e->currentTool = TOOL_SPAWN_POINT;
//				e->toolBarState = TOOLBAR_STATE_DEFAULT;
//				state_stack_push(&e->mode, EDITOR_MODE_EDIT);
//			}
//
//			if (input_is_pressed(i, KEY_i)) {
//				e->currentTool = TOOL_TILE;
//				e->toolBarState = TOOLBAR_STATE_DEFAULT;
//				state_stack_push(&e->mode, EDITOR_MODE_EDIT);
//			}
//
//			if (input_is_pressed(i, KEY_m)) {
//				e->currentTool = TOOL_ITEM;
//				e->toolBarState = TOOLBAR_STATE_DEFAULT;
//				state_stack_push(&e->mode, EDITOR_MODE_EDIT);
//			}
//
//			break;
//		case TOOLBAR_STATE_DEBUG:
//			if (input_is_pressed(i, KEY_g)) {
//				e->showGrid = !e->showGrid;
//			}
//			break;
//		default:
//			break;
//	}
//}
//
//static void
//editor_handle_input_edit(Editor* e, GameData* d, Graphics* g, Input* i,
SceneData* s) {
//	const bool modKey = input_is(i, GAME_INPUT_CTRL, INPUT_STATE_DOWN);
//	if (modKey && input_is_pressed(i, KEY_q)) {
//		state_stack_pop(&e->mode);
//		return;
//	}
//
//	const bool deletePressed = input_is_pressed(i, KEY_BACKSPACE);
//	printf("Delete is pressed: %s\n", deletePressed ? "true" : "false");
//	if (deletePressed) {
//		printf("Delete pressed: %b");
//	}
//
//	switch (e->currentTool) {
//		case TOOL_SELECT:
//		case TOOL_WALL:
//			if (deletePressed && e->selectedEntity) {
//				puts("removing wall");
//				entities_wall_remove(s, e->selectedEntity);
//				e->selectedEntity = NULL;
//			}
//
//			if (e->currentTool == TOOL_WALL &&
//					!e->selectedEntity &&
//					input_mouse_is(i, INPUT_STATE_RELEASED))
{
//				int x1 = e->startX, y1 = e->startY;
//				int x2 = e->curX, y2 = e->curY;
//				float x = x1 <= x2 ? x1 : x2;
//				float y = y1 <= y2 ? y1 : y2;
//				int xp = x1 > x2 ? x1 : x2;
//				int yp = y1 > y2 ? y1 : y2;
//				unsigned short w = xp - x;
//				unsigned short h = yp - y;
//				entities_wall_add(s, x, y, w, h);
//			}
//			break;
//		case TOOL_TEXT_INTERACTION:
//			if (e->selectedEntity) {
//				state_stack_push(&e->mode,
EDITOR_MODE_REQUEST_RESOURCE);
//
//				int size =
//					entities_text_interaction_get(s,
e->selectedEntity, e->textBuffer);
//				if (!size) {
//					bzero(e->textBuffer, sizeof(char) *
1024);
//				}
//				game_request_open_text_editor(d, e->textBuffer);
//			}
//			break;
//		case TOOL_SPAWN_POINT:
//			if (i->mouseState == INPUT_STATE_PRESSED) {
//				entities_spawn_point_set(s, (e->curX / 32) * 32,
(e->curY / 32) *32);
//			}
//			break;
//		case TOOL_TILE:
//			if (input_is_pressed(i, KEY_s)) {
//				game_request_open_tile_picker(d,
&e->tileEditor.tile);
//			}
//
//			if (input_is_pressed(i, KEY_SPACE)) {
//				if (e->tileEditor.layer == LAYER_BACKGROUND) {
//					e->tileEditor.layer = LAYER_MIDGROUND;
//				} else if (e->tileEditor.layer ==
LAYER_MIDGROUND) {
//					e->tileEditor.layer = LAYER_FOREGROUND;
//				} else {
//					e->tileEditor.layer = LAYER_BACKGROUND;
//				}
//			}
//			e->tileEditor.x = (e->curX / 32) * 32;
//			e->tileEditor.y = (e->curY / 32) * 32;
//
//			if (i->mouseState == INPUT_STATE_PRESSED ||
//					i->mouseState == INPUT_STATE_DOWN) {
//				int col = e->tileEditor.x / 32;
//				int row = e->tileEditor.y / 32;
//				int tile = row * 19 + col;
//
//				switch (e->tileEditor.layer) {
//					case LAYER_BACKGROUND:
//						s->background[tile] =
e->tileEditor.tile;
//						break;
//					case LAYER_MIDGROUND:
//						s->midground[tile] =
e->tileEditor.tile;
//						break;
//					case LAYER_FOREGROUND:
//						s->foreground[tile] =
e->tileEditor.tile;
//						break;
//				}
//			}
//			break;
//		case TOOL_ITEM:
//			if (e->selectedEntity) {
//				state_stack_push(&e->mode,
EDITOR_MODE_REQUEST_RESOURCE);
//				bzero(e->textBuffer, sizeof(char) * 1024);
//				game_request_open_text_editor(d, e->textBuffer);
//				game_request_open_tile_picker(d, &e->result);
//			}
//			break;
//		default:
//			break;
//	}
//}
//
//static void
//editor_handle_input(Editor* e, GameData* d, Graphics* g, Input* i,
SceneData* s) {
//	if (!e->isInitialzed) {
//		e->isInitialzed = true;
//		state_stack_init(&e->mode);
//		e->selectedEntity = NULL;
//		e->toolBarState = TOOLBAR_STATE_DEFAULT;
//		e->tileEditor = { .x = 0, .y = 0, .layer = LAYER_BACKGROUND };
//	}
//
//	const EditorMode mode = (EditorMode) state_stack_peek(&e->mode);
//	e->currentMode = mode < 0 ? EDITOR_MODE_NORMAL : mode;
//
//	e->curX = i->mouseX;
//	e->curY = i->mouseY;
//
//	if (input_mouse_is(i, INPUT_STATE_PRESSED)) {
//		e->startX = i->mouseX;
//		e->startY = i->mouseY;
//		e->toolBarState = TOOLBAR_STATE_DEFAULT;
//
//		e->selectedEntity = -1;
//		if (Entity* en = entities_get_by_point(s, e->startX, e->startY))
{
//			e->selectedEntity = en;
//			e->relX = e->startX - e->selectedEntity->x;
//			e->relY = e->startY - e->selectedEntity->y;
//		} else if (input_mouse_is(i, INPUT_STATE_RELEASED)) {
//			e->curX = i->mouseX;
//			e->curY = i->mouseY;
//		} else if (input_mouse_is(i, INPUT_STATE_UP)) {
//			e->startX = i->mouseX;
//			e->startY = i->mouseY;
//			e->curX = i->mouseX;
//			e->curY = i->mouseY;
//		}
//
//		const bool snapToGrid = input_is_down(i, GAME_INPUT_CMD) ||
input_is_down(i, GAME_INPUT_CTRL);
//		if (snapToGrid) {
//			e->startX = (e->startX / 32) * 32;
//			e->startY = (e->startY / 32) * 32;
//			e->curX = (e->curX / 32) * 32;
//			e->curY = (e->curY / 32) * 32;
//		}
//
//		e->isDragging = input_mouse_is(i, INPUT_STATE_DOWN) &&
distance(e->startX, e->startY, e->curX, e->curY) > 2;
//		if (e->isDragging && e->selectedEntity) {
//			e->selectedEntity->x = e->curX - e->relX;
//			e->selectedEntity->y = e->curY - e->relY;
//
//			if (snapToGrid) {
//				e->selectedEntity->x =
((int)e->selectedEntity->x / 32) * 32;
//				e->selectedEntity->y =
((int)e->selectedEntity->y / 32) * 32;
//			}
//		}
//
//		switch (e->currentMode) {
//			case EDITOR_MODE_EDIT:
//				editor_handle_input_edit(e, d, g, i, s);
//				break;
//			case EDITOR_MODE_REQUEST_RESOURCE:
//				state_stack_pop(&e->mode);
//				if (e->currentTool == TOOL_TEXT_INTERACTION) {
//					entities_text_interaction_add(s,
e->selectedEntity, e->textBuffer);
//					e->selectedEntity = NULL;
//				} else if (e->currentTool == TOOL_ITEM) {
//					entities_item_add(s, e->selectedEntity,
e->result, e->textBuffer);
//					e->selectedEntity = NULL;
//				}
//				break;
//			case EDITOR_MODE_NORMAL:
//			default:
//				editor_handle_input_normal(e, d, g, i, s);
//				break;
//		}
//	}
//
//	static void
//		editor_draw(Editor* e, Graphics* g, float timeStep) {
//			if (e->showGrid) {
//				graphics_draw_grid_overlay(g);
//			}
//
//			if (e->currentMode == EDITOR_MODE_EDIT) {
//				switch (e->currentTool) {
//					case TOOL_SELECT:
//					case TOOL_WALL:
//						if (e->selectedEntity) {
//							graphics_draw_box(g,
e->selectedEntity->x, e->selectedEntity->y,
//
e->selectedEntity->w, e->selectedEntity->h,
// COLOR_BLUE, 100);
//						}
//						if (e->isDragging &&
!e->selectedEntity) {
//
graphics_draw_selection(g, e->startX, e->startY, e->curX, e->curY);
//						}
//						break;
//					case TOOL_SPAWN_POINT:
//						graphics_draw_box(g, (e->curX /
32) * 32, (e->curY / 32) * 32,
//								32, 32,
//								COLOR_BLUE,
100);
//						break;
//					case TOOL_TILE:
//						graphics_draw_tile(g, 0,
// e->tileEditor.tile,
//
e->tileEditor.x, e->tileEditor.y, 32, 32);
//						break;
//					default:
//						break;
//				}
//				graphics_draw_text_font(g, 0, 0, 24, "Press
CTRL+q to exit mode");
//			} else {
//				graphics_draw_box(g, 0, 0, SCREEN_WIDTH, 24,
COLOR_BLUE, 255);
//				graphics_draw_text_font(g, 0, 0, 24, "File |
Tools | Debug");
//
//				switch (e->toolBarState) {
//					case TOOLBAR_STATE_FILE:
//						graphics_draw_menu(g, 0, 24, 24,
fileMenu, 2);
//						break;
//					case TOOLBAR_STATE_TOOL:
//						graphics_draw_menu(g, 66, 24,
24, toolMenu, 6);
//						break;
//					case TOOLBAR_STATE_DEBUG:
//						graphics_draw_menu(g, 180, 24,
24, debugMenu, 1);
//						break;
//					default:
//						break;
//				}
//			}
//		}
//
//static void
//player_process_movement(Body* body, Velocity* vel, const float timeStep);
//
//
//char* mainMenu[] = {
//    "Continue",
//    "New Game",
//    "Options"
//};
//
//int mainMenuResult;
//
void*
game_init() {
        GameData* d = malloc(sizeof(GameData));
        bzero(d, sizeof(GameData));

        if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO) < 0) {
                DEBUG(SDL_GetError());
                return NULL;
        }

        d->window = SDL_CreateWindow
                (
                 "RPG",
                 SDL_WINDOWPOS_UNDEFINED,
                 SDL_WINDOWPOS_UNDEFINED,
                 SCREEN_WIDTH,
                 SCREEN_HEIGHT,
                 SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
                );

        if (d->window == NULL) {
                DEBUG(SDL_GetError());
                return NULL;
        }

        d->renderer = SDL_CreateRenderer(
                        d->window,
                        -1,
                        SDL_RENDERER_ACCELERATED |
SDL_RENDERER_PRESENTVSYNC); if (d->renderer == NULL) {
                DEBUG(SDL_GetError());
                return NULL;
        }

        if (TTF_Init() < 0) {
                DEBUG(SDL_GetError());
                return NULL;
        }

        d->font = TTF_OpenFont("resources/slkscr.ttf", 16);
        if (d->font == NULL) {
                DEBUG("COULD NOT OPEN FONT resources/slkscr.ttf");
                return NULL;
        }

        if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)) {
                DEBUG(Mix_GetError());
                return NULL;
        }

        //bzero(&d->input, sizeof(Input));

        //state_stack_init(&d->gameState);
        //state_stack_push(&d->gameState, GAME_STATE_NORMAL);
        //// TODO: Reimplement
        ////tilePicker.tilesetMeta.id = 0;
        ////tilePicker.tilesetMeta.hTiles = ((g->textureCache[0].w - 16) /
17) + 1;
        ////tilePicker.tilesetMeta.vTiles = ((g->textureCache[0].h - 16) /
17) + 1;
        ////tilePicker.tilesetMeta.totalTiles =
        ////    tilePicker.tilesetMeta.hTiles *
tilePicker.tilesetMeta.vTiles;
        ////tilePicker.tile = 0;
        //d->editor.isInitialzed = false;

        //for (int j = 0; j < d->scene.backgroundSize; j++) {
        //	d->scene.background[j] = -1;
        //	d->scene.midground[j] = -1;
        //	d->scene.foreground[j] = -1;
        //}
        //d->scene.spawnPoint.x = -1;
        //d->scene.spawnPoint.y = -1;
        //scene_load(&d->scene);
        //Body b = { d->scene.spawnPoint.x, d->scene.spawnPoint.y, 32, 32 };
        //d->scene.bodies[0] = b;
        //d->scene.vel.xVel = 0;
        //d->scene.vel.yVel = 0;

        //state_stack_push(&d->gameState, GAME_STATE_STARTUP);
        //d->lastTime = 0.0f;
        //d->frame = 0L;

        return d;
}
//
long
game_run_frame(GameData* d) {
        puts("Running!");
        float currentTime = ((float)SDL_GetTicks()) / 1000;
        float timeStep = currentTime - d->lastTime;
        d->lastTime = currentTime;

        //Audio* a = &d->audio;
        Input* i = &d->input;
        if (input_process(i)) {
                return -1;
        }

        if (input_key_is_pressed(i, SDLK_r)) {
                d->showFrameRate = !d->showFrameRate;
        }

        //if (input_is_pressed(i, KEY_e)
        //		&& state_stack_peek(&d->gameState) == GAME_STATE_NORMAL)
{
        //	state_stack_push(&d->gameState, GAME_STATE_EDITOR);
        //}

        //if (input_is_pressed(i, GAME_INPUT_ESC)
        //		&& state_stack_peek(&d->gameState) == GAME_STATE_EDITOR)
{
        //	state_stack_pop(&d->gameState);
        //}

        //switch (state_stack_peek(&d->gameState)) {
        //	case GAME_STATE_STARTUP:
        //		if (input_is_pressed(i, GAME_INPUT_SELECT)) {
        //			audio_queue_sound(a, 0);
        //			if (d->fadeIn < 6.5f) {
        //				d->fadeIn = 6.5f;
        //			} else {
        //				d->startSelected = true;
        //			}
        //		}

        //		if (input_is_pressed(i, GAME_INPUT_BACK)) {
        //			state_stack_pop(&d->gameState);
        //			audio_request_stop_music(a);
        //		}

        //		if (d->startSelectedFlash > 2.0f) {
        //			state_stack_pop(&d->gameState);
        //			Body m_size = { 0, 0, SCREEN_WIDTH,
SCREEN_HEIGHT };
        //			Point textStartingPoint = { 224.0f, 128.0f };
        //			const char* menuTitle = "Main Menu";
        //			game_request_open_modal
        //				(
        //				 d,
        //				 &m_size,
        //				 &textStartingPoint,
        //				 menuTitle,
        //				 mainMenu,
        //				 3,
        //				 &mainMenuResult
        //				);
        //			audio_request_stop_music(a);
        //		}
        //		break;
        //	case GAME_STATE_EDITOR:
        //		editor_handle_input(&d->editor, d, g, i, &d->scene);
        //		break;
        //	case GAME_STATE_TEXTBOX:
        //		if (input_is_pressed(i, GAME_INPUT_SELECT)) {
        //			state_stack_pop(&d->gameState);
        //		}
        //		break;
        //	case GAME_STATE_MODAL:
        //		if (modal_handle_input(i, &d->modal)) {
        //			state_stack_pop(&d->gameState);
        //		}
        //		break;
        //	case GAME_STATE_TILE_PICKER:
        //		if (tile_picker_handle_input(i, &d->tilePicker)) {
        //			*d->tile = d->tilePicker.tile;
        //			state_stack_pop(&d->gameState);
        //		}
        //		break;
        //	case GAME_STATE_TEXT_EDITOR:
        //		if (text_editor_handle_input(&d->textEditor, i)) {
        //			state_stack_pop(&d->gameState);
        //		}
        //	case GAME_STATE_NORMAL:
        //	default:
        //		const int MAX_VELOCITY = 4;
        //		if (input_is_down(i, GAME_INPUT_LEFT)) {
        //			d->scene.vel.xVel = -MAX_VELOCITY;
        //		}
        //		if (input_is_down(i, GAME_INPUT_RIGHT)) {
        //			d->scene.vel.xVel = MAX_VELOCITY;
        //		}
        //		if (input_is_down(i, GAME_INPUT_UP)) {
        //			d->scene.vel.yVel = -MAX_VELOCITY;
        //		}
        //		if (input_is_down(i, GAME_INPUT_DOWN)) {
        //			d->scene.vel.yVel = MAX_VELOCITY;
        //		}

        //		scene_process_interaction(d, &d->scene, i);
        //		break;
        //}

        //if (state_stack_peek(&d->gameState) == GAME_STATE_NORMAL) {
        //	Body* player = entities_get_body(&d->scene, 0);
        //	float startX = player->x;
        //	float startY = player->y;
        //	player_process_movement(player, &d->scene.vel, timeStep);

        //	for (int i = 0; i < MAX_ENTITIES; i++) {
        //		Entity* e = &s.entities;
        //		if (e->id > 0 && e->isSolid &&
utils_entities_collide(player, &e->body)) {
        //			player->x = startX;
        //			player->y = startY;
        //			break;
        //		}
        //	}
        //}

        //graphics_draw_box(g, 0, 0, 1000, 1000, COLOR_BLACK, 255);
        //// Draw level
        //graphics_draw_tiles(g, d->scene.tileSet, d->scene.background,
d->scene.backgroundSize);
        //graphics_draw_tiles(g, d->scene.tileSet, d->scene.midground,
d->scene.midgroundSize);
        //for (int i = 0; i < MAX_ENTITIES; i++) {
        //	Entity* e = &d->scene.entities[i]
        //		if (e->id > 0 && e->sprite > 0) {
        //			graphics_draw_tile(g, d->scene.tileSet,
e->sprite, e->body->x, e->body->y, e->body->w, e->body->h);
        //		}
        //}

        //if (state_stack_peek(&d->gameState) == GAME_STATE_EDITOR) {
        //	for (int i = 0; i < MAX_ENTITIES; i++) {
        //		Entity* e = &d->scene.entities[i];
        //		if (e->id > 0 && e->isSolid) {
        //			graphics_draw_box(g, e->body->x, e->body->y,
e->body->w, e->body->h, COLOR_WHITE, 100);
        //		}
        //	}

        //	graphics_draw_box(g, d->scene.spawnPoint.x,
d->scene.spawnPoint.y, 32, 32, COLOR_BLUE, 100);
        //}
        //Body* b = entities_get_body(&d->scene, 0);
        //graphics_draw_box(g, b->x, b->y, b->w, b->h, COLOR_BLUE, 255);
        //if (d->foundItem > 0.0f) {
        //	graphics_draw_tile(g, 0, d->interactionData.item.tile, b->x,
b->y - 40, 32, 32);
        //	d->foundItem -= timeStep;
        //}
        //graphics_draw_tiles(g, d->scene.tileSet, d->scene.foreground,
d->scene.foregroundSize);

        if (d->showFrameRate) {
                char b[4];
                sprintf(b, "%d", (int) (1.0f / timeStep));
                graphics_draw_text(g, 0, 0, 60, 30, b);
        }

        //if (state_stack_peek(&d->gameState) == GAME_STATE_TEXTBOX) {
        //	const int y = b->y > 256 ? 0 : 256;

        //	graphics_draw_box(g, 0, y, 608, 160, COLOR_BLUE, 255);
        //	graphics_draw_texture(g, d->textBox.textureId, 0, 0, 160, 160);
        //	graphics_draw_wrapped_text(g, 192, y, 32, 384, d->textBox.text);
        //}

        //if (state_stack_peek(&d->gameState) == GAME_STATE_EDITOR) {
        //	editor_draw(&d->editor, g,timeStep);
        //}

        //if (state_stack_peek(&d->gameState) == GAME_STATE_MODAL) {
        //	modal_draw(g, &d->modal, timeStep);
        //}

        //if (state_stack_peek(&d->gameState) == GAME_STATE_TILE_PICKER) {
        //	tile_picker_draw(g, &d->tilePicker);
        //}

        //if (state_stack_peek(&d->gameState) == GAME_STATE_TEXT_EDITOR) {
        //	graphics_draw_box(g, 0, 0, 1000, 1000, COLOR_BLUE, 255);
        //	graphics_draw_wrapped_text(g, 0, 0, 32, 608,
d->textEditor.buffer);
        //}

        //if (state_stack_peek(&d->gameState) == GAME_STATE_STARTUP) {
        //	if (d->fadeIn < 6.5f) {
        //		graphics_draw_texture(g, 1, 0, 0, SCREEN_WIDTH,
SCREEN_HEIGHT);
        //		graphics_draw_box(g, 0, 0,
        //				SCREEN_WIDTH,
        //				SCREEN_HEIGHT,
        //				COLOR_BLACK,
        //				(int)(255.0f - ((255.0f / 6.5f) *
d->fadeIn)));
        //		if (d->fadeIn > 6.0f) {
        //			graphics_draw_box(g, 0, 0,
        //					SCREEN_WIDTH,
        //					SCREEN_HEIGHT,
        //					COLOR_WHITE,
        //					(int)((255.0f / 0.5f) *
(d->fadeIn - 6.0f)));
        //		}
        //	} else {
        //		graphics_draw_texture(g, 1, 0, 0, SCREEN_WIDTH,
SCREEN_HEIGHT);
        //		graphics_draw_text_font(g, SCREEN_WIDTH / 2 - 32, 64,
48, "Vulfy Story");

        //		float junk;
        //		if (d->startSelected && (modff(d->startSelectedFlash /
0.25, &junk) < 0.5f)) {
        //			graphics_draw_text_font(g, SCREEN_WIDTH / 2 -
32, SCREEN_HEIGHT / 2 + 96, 48, "Press Start");
        //		} else if (!d->startSelected &&
(modff(d->flashingButton, &junk) < 0.5f)) {
        //			graphics_draw_text_font(g, SCREEN_WIDTH / 2 -
32, SCREEN_HEIGHT / 2 + 96, 48, "Press Start");
        //		}
        //	}

        //	if (d->startSelected) {
        //		d->startSelectedFlash += timeStep;
        //	}
        //	d->flashingButton += timeStep;
        //	d->fadeIn += timeStep;
        //}

        //if (d->openTextBoxRequested) {
        //	d->openTextBoxRequested = false;
        //	state_stack_push(&d->gameState, GAME_STATE_TEXTBOX);
        //} else if (d->openModalRequested) {
        //	d->openModalRequested = false;
        //	state_stack_push(&d->gameState, GAME_STATE_MODAL);
        //} else if (d->openTilePickerRequested) {
        //	d->openTilePickerRequested = false;
        //	state_stack_push(&d->gameState, GAME_STATE_TILE_PICKER);
        //} else if (d->openTextEditorRequested) {
        //	d->openTextEditorRequested = false;
        //	state_stack_push(&d->gameState, GAME_STATE_TEXT_EDITOR);
        //} else if (d->sceneSaveRequested) {
        //	d->sceneSaveRequested = false;
        //	scene_save(&d->scene);
        //} else if (d->sceneLoadRequested) {
        //	d->sceneLoadRequested = false;
        //	scene_load(&d->scene);
        //}



        //// Process audio and graphics
        //audio_process(a);
        //graphics_present(g);

        ////	graphics_shutdown(&graphics);
        ////	audio_shutdown(&audio);
        SDL_Delay(1000 / 60);
        d->frame++;
        return d->frame;
}
//
//static void
//player_process_movement(Body* body, Velocity* vel, const float timeStep) {
//    float startX = body->x;
//    float startY = body->y;
//    int xVelocity = vel->xVel;
//    int yVelocity = vel->yVel;
//    if (xVelocity < 0) {
//        body->x += -120 * timeStep;
//    	vel->xVel += 2;
//    } else if (xVelocity > 0) {
//    	body->x += 120 * timeStep;
//    	vel->xVel += -2;
//    }
//
//    if (yVelocity < 0) {
//    	body->y += -120 * timeStep;
//    	vel->yVel += 2;
//    } else if (yVelocity > 0) {
//    	body->y += 120 * timeStep;
//    	vel->yVel += -2;
//    }
//
//    if (startX < -30) {
//        body->x = SCREEN_WIDTH + 30;
//    } else if (startX > SCREEN_WIDTH + 30) {
//    	body->x = -30;
//    }
//
//    if (startY < -30) {
//    	body->y = SCREEN_HEIGHT + 30;
//    } else if (startY > SCREEN_HEIGHT + 30) {
//    	body->y = -30;
//    }
//}
*/
