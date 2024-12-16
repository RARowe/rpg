SRC_DIR := src
CFLAGS := -g -Wall -std=c99
LDLIBS	 := -I/usr/local/include/SDL2 -L. -L/usr/local/lib -L/lib/x86_64-linux-gnu -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

main: libgame.so $(SRC_DIR)/main.c
	gcc $(CFLAGS) $(SRC_DIR)/main.c -o main $(LDLIBS)

libgame.so: src/game.c src/entities.c src/editor.c src/input.c
	gcc $(CFLAGS) -fPIC -shared $(SRC_DIR)/game.c -o libgame.so $(LDLIBS)

.PHONY: clean
clean:
	rm -f main libgame.so
