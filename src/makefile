# SRC_DIR := src
CFLAGS := -g -Wall -std=c89 -pedantic
 LDLIBS	 := -I/usr/local/include/SDL2 -lSDL2 -lSDL2_image -lSDL2_ttf
 # -lSDL2_mixer -lSDL2_ttf
# 
main: main.c game.o
	gcc $(CFLAGS) main.c game.o -o main $(LDLIBS)

game.o: game.c
	gcc $(CFLAGS) -c game.c

# libgame.so: src/game.c src/entities.c src/editor.c src/input.c
# 	gcc $(CFLAGS) -fPIC -shared $(SRC_DIR)/game.c -o libgame.so $(LDLIBS)

.PHONY: clean
clean:
	rm -f main game.o
