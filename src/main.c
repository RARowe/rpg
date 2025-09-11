#include <assert.h>
#include <stdio.h>

#include "types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
void process_graphics(SDL_Renderer *r, SDL_Texture *t, TTF_Font *f, GBuffer *g);

#define ONE_MB 1024 * 1024 * 1024

int main() {
    SDL_Window *w;
    SDL_Renderer *r;
    SDL_Surface *s;
    SDL_Texture *t;
    TTF_Font *f;
    GBuffer *g;
    IBuffer *in;
    MBuffer m;
    int running;
    float lastTime, currentTime, ts;

    if (!(w = SDL_CreateWindow(
              "RPG", SDL_WINDOWPOS_UNDEFINED | SDL_RENDERER_PRESENTVSYNC,
              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
              SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI))) {
        perror(SDL_GetError());
        return 1;
    }

    if (!(r = SDL_CreateRenderer(w, 1, SDL_RENDERER_ACCELERATED))) {
        perror(SDL_GetError());
        return 1;
    }

    if (TTF_Init()) {
        perror(TTF_GetError());
        return 1;
    }

    if (!(s = IMG_Load("world.png"))) {
        perror("unable to load surface");
        return 1;
    }

    if (!(t = SDL_CreateTextureFromSurface(r, s))) {
        perror(SDL_GetError());
        return 1;
    }

    SDL_FreeSurface(s);

    if (!(f = TTF_OpenFont("slkscr.ttf", 16))) {
        perror(TTF_GetError());
        return 1;
    }
    /* 628 x 425 */

    if (!(g = malloc(sizeof(GBuffer)))) {
        perror("unable to alloc memory for GBuffer");
        return 1;
    }

    if (!(in = malloc(sizeof(IBuffer)))) {
        perror("unable to alloc memory for IBuffer");
        return 1;
    }

    /* Important to init mem to 0 for first frame */
    if (!(m.m = calloc(ONE_MB, sizeof(char)))) {
        perror("unable to alloc memory for GBuffer");
        return 1;
    }
    m.size = ONE_MB;
    m.head = 0;

    /* Initial setup before game loop */
    running = 1;
    lastTime = 0.0f;
    while (running) {
        SDL_Event e;

        currentTime = ((float)SDL_GetTicks()) / 1000;
        ts = currentTime - lastTime;
        lastTime = currentTime;

        g->idx = 0;
        in->idx = 0;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }

            assert(in->idx < BUFFER_SIZE);

            if (e.type == SDL_KEYDOWN) {
                in->instructions[in->idx].type = IDOWN;
            } else if (e.type == SDL_KEYUP) {
                in->instructions[in->idx].type = IUP;
            } else if (e.type == SDL_MOUSEMOTION) {
                in->instructions[in->idx].type = IMOUSEMOTION;
                in->instructions[in->idx].x = e.motion.x;
                in->instructions[in->idx].y = e.motion.y;
                in->idx += 1;
                /*case SDL_MOUSEBUTTONDOWN/UP:*/
                continue;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                in->instructions[in->idx].type = IMOUSEDOWN;
                in->instructions[in->idx].x = e.button.x;
                in->instructions[in->idx].y = e.button.y;
                in->idx += 1;
            } else {
                continue;
            }

            switch (e.key.keysym.sym) {
            case SDLK_w:
                in->instructions[in->idx].action = IFORWARD;
                break;
            case SDLK_a:
                in->instructions[in->idx].action = ILEFT;
                break;
            case SDLK_s:
                in->instructions[in->idx].action = IBACK;
                break;
            case SDLK_d:
                in->instructions[in->idx].action = IRIGHT;
                break;
            case SDLK_j:
                in->instructions[in->idx].action = IACTION;
                break;
            case SDLK_ESCAPE:
                in->instructions[in->idx].action = IEXIT;
                break;
            default:
                continue;
            }

            in->idx += 1;
        }
        game_run(g, in, &m, ts);
        process_graphics(r, t, f, g);
        SDL_Delay(16);
    }

    return 0;
}

void process_graphics(SDL_Renderer *r, SDL_Texture *tileMap, TTF_Font *f,
                      GBuffer *g) {
    int i;
    SDL_Rect out;
    SDL_Surface *s;
    SDL_Texture *t;
    SDL_Color color;

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    out.x = 0, out.y = 0, out.w = SCREEN_WIDTH, out.h = SCREEN_HEIGHT;
    SDL_RenderFillRect(r, &out);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

    for (i = 0; i < g->idx; i++) {
        switch (g->instructions[i].type) {
        case GTEXT: {
            char *text;
            text = g->instructions[i].action.GText.text;

            memcpy(&out, &g->instructions[i].action.GText, sizeof(SDL_Rect));
            color.r = 255;
            color.g = 255;
            color.b = 255;
            color.a = 255;
            s = TTF_RenderText_Solid(f, text, color);
            assert(s != NULL);
            t = SDL_CreateTextureFromSurface(r, s);
            assert(t != NULL);
            assert(SDL_RenderCopy(r, t, NULL, &out) == 0);
            SDL_DestroyTexture(t);
            SDL_FreeSurface(s);
            break;
        }
        case GTILE: {
            SDL_Rect in;
            int x, y, w, h, tile;
            tile = g->instructions[i].action.GTile.tile;
            x = g->instructions[i].action.GTile.x;
            y = g->instructions[i].action.GTile.y;
            w = g->instructions[i].action.GTile.w;
            h = g->instructions[i].action.GTile.h;

            out.x = x, out.y = y, out.w = w, out.h = h;
            in.x = (tile % 37) * 17, in.y = (tile / 37) * 17;
            in.w = 16, in.h = 16;

            assert(SDL_RenderCopy(r, tileMap, &in, &out) == 0);
            break;
        }
        case GTILES: {
            SDL_Rect in;
            int rs, cs, tile;
            int *tiles;
            int j, k;

            rs = g->instructions[i].action.GTiles.r;
            cs = g->instructions[i].action.GTiles.c;
            tiles = g->instructions[i].action.GTiles.tiles;
            in.x = 0, in.y = 0, in.w = 16, in.h = 16;
            out.x = 0, out.y = 0, out.w = 32, out.h = 32;
            for (j = 0; j < rs; j++) {
                for (k = 0; k < cs; k++) {
                    tile = tiles[(j * cs) + k];
                    /* 37 cols in spritesheet, 17 pixel offset */
                    in.x = (tile % 37) * 17;
                    in.y = (tile / 37) * 17;
                    out.x = k * 32;
                    out.y = j * 32;
                    assert(SDL_RenderCopy(r, tileMap, &in, &out) == 0);
                }
            }
            break;
        }
        case GBOX: {
            unsigned char red, grn, blu;
            out.x = g->instructions[i].action.GBox.x;
            out.y = g->instructions[i].action.GBox.y;
            out.w = g->instructions[i].action.GBox.w;
            out.h = g->instructions[i].action.GBox.h;
            red = g->instructions[i].action.GBox.r;
            grn = g->instructions[i].action.GBox.g;
            blu = g->instructions[i].action.GBox.b;

            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(r, red, grn, blu, 255);
            SDL_RenderFillRect(r, &out);
            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
            break;
        }
        case GOUTLINE: {
            unsigned char red, grn, blu;
            out.x = g->instructions[i].action.GBox.x;
            out.y = g->instructions[i].action.GBox.y;
            out.w = g->instructions[i].action.GBox.w;
            out.h = g->instructions[i].action.GBox.h;
            red = g->instructions[i].action.GBox.r;
            grn = g->instructions[i].action.GBox.g;
            blu = g->instructions[i].action.GBox.b;

            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(r, red, grn, blu, 255);
            SDL_RenderDrawRect(r, &out);
            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
            break;
        }
        default:
            puts("did not match");
            return;
        }
    }
    SDL_RenderPresent(r);
}

/*
//void* handle;
//long (*_game_run_frame)(void* d);
//void *(*_game_init)();
//
//long lastAccess = 0;
//int check_reload_bin() {
//	struct stat buf;
//	if (stat("./libgame.so", &buf)) {
//		return 1;
//	}
//
//	if (buf.st_mtime <= lastAccess) {
//		return 1;
//	}
//	lastAccess = buf.st_mtime;
//
//
//	void* tempHandle;
//	if ((tempHandle = dlopen("./libgame.so", RTLD_LAZY)) == NULL) {
//		puts(dlerror());
//		return 0;
//	}
//
//	if ((_game_init = dlsym(tempHandle, "game_init")) == NULL) {
//		puts(dlerror());
//		return 0;
//	}
//
//	if ((_game_run_frame = dlsym(tempHandle, "game_run_frame")) == NULL) {
//		puts(dlerror());
//		return 0;
//	}
//
//	if (handle != NULL) {
//		if (dlclose(handle)) {
//			puts(dlerror());
//			return 0;
//		}
//	}
//
//	return 1;
//}
//
//int main() {
//	if (!check_reload_bin()) {
//		puts("unable to load libgame.so");
//		return EXIT_FAILURE;
//	}
//
//	void* data = _game_init();
//
//	long frame = 0L;
//	while (1) {
//		if ((frame = _game_run_frame(data)) == -1) {
//			break;
//		}
//
//		if (frame % 60 == 0) {
//			check_reload_bin();
//		}
//	}
//
//	return 0;
//}
*/
