#include <dlfcn.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

void* handle;
long (*_game_run_frame)(void* d);
void *(*_game_init)();

long lastAccess = 0;
int check_reload_bin() {
	struct stat buf;
	if (stat("./libgame.so", &buf)) {
		return 1;
	}

	if (buf.st_mtim.tv_sec <= lastAccess) {
		return 1;
	}
	lastAccess = buf.st_mtim.tv_sec;
	

	void* tempHandle;
	if ((tempHandle = dlopen("./libgame.so", RTLD_LAZY)) == NULL) {
		puts(dlerror());
		return 0;
	}

	if ((_game_run_frame = dlsym(tempHandle, "game_run_frame")) == NULL) {
		puts(dlerror());
		return 0;
	}

	if ((_game_init = dlsym(tempHandle, "game_init")) == NULL) {
		puts(dlerror());
		return 0;
	}

	if (handle != NULL) {
		if (dlclose(handle)) {
			puts(dlerror());
			return 0;
		}
	}
	handle == tempHandle;

	return 1;
}

int main() {
	if (!check_reload_bin()) {
		puts("unable to load libgame.so");
		return EXIT_FAILURE;
	}

	void* data = _game_init();

	long frame = 0L;
	while (1) {
		frame = _game_run_frame(data);

		if (frame % 60 == 0) {
			check_reload_bin();
		}
	}

	return 0;
}
