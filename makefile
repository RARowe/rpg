SRC_DIR := src
OBJ_DIR := object
# -Wall
CXXFLAGS := -std=c99
LDLIBS	 := -I/usr/local/include/SDL2 -L. -L/usr/local/lib -L/lib/x86_64-linux-gnu -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lstdc++

.PHONY: clean

main: libgame.so $(SRC_DIR)/main.c
	gcc -g -Wall -fpermissive $(SRC_DIR)/main.c -o main $(LDLIBS)

libgame.so: src/game.c src/entities.c src/editor.c src/game.h
	gcc -g $(CXXFLAGS) -fPIC -shared $(SRC_DIR)/game.c -o libgame.so $(LDLIBS)

clean:
	rm -f main libgame.so
