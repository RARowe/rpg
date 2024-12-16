#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_HEIGHT 416
#define SCREEN_WIDTH 608
#define DEBUG puts

// Get all types first
#include "types.h"

// Platform
#include "audio.c"
#include "input.c"
#include "graphics.c"

// Needed
//static void
//game_request_open_text_box(GameData* d, unsigned int textureId, const char* text) {
//    d->openTextBoxRequested = true;
//    d->textBox.textureId = textureId;
//    strcpy(d->textBox.text, text);
//}
//
//static void
//game_request_open_modal(GameData* d, const Body* size, const Point* textStartingPoint, const char* title, char** options, int numberOfOptions, int* result) {
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
//#include "state_stack.c"
//#include "entities.c"
//#include "modal.c"
//#include "scene.c"
//#include "editor.c"
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
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (d->renderer == NULL) {
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
	////tilePicker.tilesetMeta.hTiles = ((g->textureCache[0].w - 16) / 17) + 1;
	////tilePicker.tilesetMeta.vTiles = ((g->textureCache[0].h - 16) / 17) + 1;
	////tilePicker.tilesetMeta.totalTiles =
	////    tilePicker.tilesetMeta.hTiles * tilePicker.tilesetMeta.vTiles;
	////tilePicker.tile = 0;
	//d->editor.isInitialzed = false;

	///* TODO: Remove this */
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
	///* End */

	///* Show opening credits */
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
	///* Handle Input */
	//if (input_is_pressed(i, KEY_r)) {
	//	d->showFrameRate = !d->showFrameRate;
	//}

	//if (input_is_pressed(i, KEY_e)
	//		&& state_stack_peek(&d->gameState) == GAME_STATE_NORMAL) {
	//	state_stack_push(&d->gameState, GAME_STATE_EDITOR);
	//}

	//if (input_is_pressed(i, GAME_INPUT_ESC)
	//		&& state_stack_peek(&d->gameState) == GAME_STATE_EDITOR) {
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
	//			Body m_size = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
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
	///* End */

	//if (state_stack_peek(&d->gameState) == GAME_STATE_NORMAL) {
	//	Body* player = entities_get_body(&d->scene, 0);
	//	float startX = player->x;
	//	float startY = player->y;
	//	player_process_movement(player, &d->scene.vel, timeStep);

	//	for (int i = 0; i < MAX_ENTITIES; i++) {
	//		Entity* e = &s.entities;
	//		if (e->id > 0 && e->isSolid && utils_entities_collide(player, &e->body)) {
	//			player->x = startX;
	//			player->y = startY;
	//			break;
	//		}
	//	}
	//}

	//graphics_draw_box(g, 0, 0, 1000, 1000, COLOR_BLACK, 255);
	//// Draw level
	//graphics_draw_tiles(g, d->scene.tileSet, d->scene.background, d->scene.backgroundSize);
	//graphics_draw_tiles(g, d->scene.tileSet, d->scene.midground, d->scene.midgroundSize);
	//for (int i = 0; i < MAX_ENTITIES; i++) {
	//	Entity* e = &d->scene.entities[i]
	//		if (e->id > 0 && e->sprite > 0) {
	//			graphics_draw_tile(g, d->scene.tileSet, e->sprite, e->body->x, e->body->y, e->body->w, e->body->h);
	//		}
	//}

	//if (state_stack_peek(&d->gameState) == GAME_STATE_EDITOR) {
	//	for (int i = 0; i < MAX_ENTITIES; i++) {
	//		Entity* e = &d->scene.entities[i];
	//		if (e->id > 0 && e->isSolid) {
	//			graphics_draw_box(g, e->body->x, e->body->y, e->body->w, e->body->h, COLOR_WHITE, 100);
	//		}
	//	}

	//	graphics_draw_box(g, d->scene.spawnPoint.x, d->scene.spawnPoint.y, 32, 32, COLOR_BLUE, 100);
	//}
	///* Player Rendering */
	//Body* b = entities_get_body(&d->scene, 0);
	//graphics_draw_box(g, b->x, b->y, b->w, b->h, COLOR_BLUE, 255);
	//if (d->foundItem > 0.0f) {
	//	graphics_draw_tile(g, 0, d->interactionData.item.tile, b->x, b->y - 40, 32, 32);
	//	d->foundItem -= timeStep;
	//}
	//graphics_draw_tiles(g, d->scene.tileSet, d->scene.foreground, d->scene.foregroundSize);
	///* End */

	//if (d->showFrameRate) {
	//	char b[4];
	//	sprintf(b, "%d", (int) (1.0f / timeStep));
	//	graphics_draw_text(g, 0, 0, 60, 30, b);
	//}

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
	//	graphics_draw_wrapped_text(g, 0, 0, 32, 608, d->textEditor.buffer);
	//}

	//if (state_stack_peek(&d->gameState) == GAME_STATE_STARTUP) {
	//	if (d->fadeIn < 6.5f) {
	//		graphics_draw_texture(g, 1, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//		graphics_draw_box(g, 0, 0,
	//				SCREEN_WIDTH,
	//				SCREEN_HEIGHT,
	//				COLOR_BLACK,
	//				(int)(255.0f - ((255.0f / 6.5f) * d->fadeIn)));
	//		if (d->fadeIn > 6.0f) {
	//			graphics_draw_box(g, 0, 0,
	//					SCREEN_WIDTH,
	//					SCREEN_HEIGHT,
	//					COLOR_WHITE,
	//					(int)((255.0f / 0.5f) * (d->fadeIn - 6.0f)));
	//		}
	//	} else {
	//		graphics_draw_texture(g, 1, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//		graphics_draw_text_font(g, SCREEN_WIDTH / 2 - 32, 64, 48, "Vulfy Story");

	//		float junk;
	//		if (d->startSelected && (modff(d->startSelectedFlash / 0.25, &junk) < 0.5f)) {
	//			graphics_draw_text_font(g, SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 + 96, 48, "Press Start");
	//		} else if (!d->startSelected && (modff(d->flashingButton, &junk) < 0.5f)) {
	//			graphics_draw_text_font(g, SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 + 96, 48, "Press Start");
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
