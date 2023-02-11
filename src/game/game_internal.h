#pragma once
#ifndef GAME_INTERNAL_H
#define GAME_INTERNAL_H

/* Exported */
typedef struct GameData {
    TextBox textBox;
    Modal modal;
    TilePicker tilePicker;
    SceneData scene;
    Editor editor;
    TextEditor textEditor;
    InteractionData interactionData;
    float foundItem;
    int *tile;
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

static void game_request_open_text_box(GameData *d, unsigned int textureId,
                                       const char *text);
static void game_request_open_modal(GameData *d, char **options,
                                    int numberOfOptions, int *result);
static void game_request_open_modal(GameData *d, const Body *size,
                                    const Point *textStartingPoint,
                                    const char *title, char **options,
                                    int numberOfOptions, int *result);
static void game_request_open_tile_picker(GameData *d, int *tile);
static void game_request_open_text_editor(GameData *d, char *buffer);
static void game_request_scene_save(GameData *d);
static void game_request_scene_load(GameData *d);

/* entities.c */
static Body *entities_get_body(SceneData *s, int entityId);
static Body *entities_get_body_by_point(SceneData *s, float x, float y);
static void entities_wall_add(SceneData *s, float x, float y, short w, short h);
static void entities_wall_remove(SceneData *s, void *entity);
static void entities_item_add(SceneData *s, void *entity, int tile,
                              const char *description);
static int entities_item_get(SceneData *s, void *entity, Item *item);
static void entities_text_interaction_add(SceneData *s, void *entity,
                                          const char *text);
static size_t entities_text_interaction_get(SceneData *s, void *entity,
                                            char *buffer);
static void entities_spawn_point_set(SceneData *s, float x, float y);

/* editor.c */
static void editor_handle_input(GameData *d, Editor *e, Graphics *g, Input *i,
                                SceneData *s);
static void editor_draw(Editor *e, Graphics *g, float timeStep);

/* modal.c */
static int modal_handle_input(const Input *i, Modal *m);
static void modal_draw(Graphics *g, const Modal *m, float timeStep);
static int tile_picker_handle_input(const Input *i, TilePicker *p);
static int text_editor_handle_input(TextEditor *t, const Input *i);
static void tile_picker_draw(Graphics *g, const TilePicker *p);
#endif