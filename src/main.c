#include "game.h"
#include "platform.h"

#include <dlfcn.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <sys/stat.h>

void* handle;
void (*_game_run_frame)(GameData* d, Graphics* g, Audio* a, Input* i, float timeStep);
void (*_game_init)(GameData* d);

long lastAccess = 0;
bool check_reload_bin() {
	struct stat buf;
	if (stat("./libgame.so", &buf)) {
		return true;
	}

	if (buf.st_mtim.tv_sec <= lastAccess) {
		return true;
	}
	lastAccess = buf.st_mtim.tv_sec;
	
	if (handle != NULL) {
		if (dlclose(handle)) {
			puts(dlerror());
			return false;
		}
	}

	if ((handle = dlopen("./libgame.so", RTLD_LAZY)) == NULL) {
		puts(dlerror());
		return false;
	}

	if ((_game_run_frame = dlsym(handle, "game_run_frame")) == NULL) {
		puts(dlerror());
		return false;
	}

	if ((_game_init = dlsym(handle, "game_init")) == NULL) {
		puts(dlerror());
		return false;
	}

	return true;
}

int main() {
	GameData data;
	Graphics graphics;
	Audio audio;
	Input input;
	if (!check_reload_bin()) {
		puts("unable to load libgame.so");
		return EXIT_FAILURE;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not be initialized! SDL Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (!graphics_init(&graphics, "RPG", SCREEN_WIDTH, SCREEN_HEIGHT, "resources/")) {
		return EXIT_FAILURE;
	}

	if (!audio_init(&audio)) {
		return EXIT_FAILURE;
	}

	bzero(&input, sizeof(Input));

	// TODO: Remove this. Game should initialize
	_game_init(&data);

	float lastTime = 0.0f;
	long frame = 0L;
	while (input_process(&input)) {
		float currentTime = ((float)SDL_GetTicks()) / 1000;
		float timeStep = currentTime - lastTime;
		lastTime = currentTime;

		_game_run_frame(&data, &graphics, &audio, &input, timeStep);

		audio_process(&audio);
		graphics_present(&graphics);

		SDL_Delay(1000 / 60);

		if (frame % 60 == 0) {
			check_reload_bin();
		}
		frame++;
	}

	graphics_shutdown(&graphics);
	audio_shutdown(&audio);
}
