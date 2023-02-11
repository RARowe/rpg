SRC_DIR := src
OBJ_DIR := object

INCLIBS  := -I/src/include -I/usr/local/include/SDL2
LDLIBS	 := -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

.PHONY: clean

main:
	gcc $(INCLIBS) $(LDLIBS) $(SRC_DIR)/platform/platform.c -o main

game.o:
	gcc -c $(INCLIBS) $(SRC_DIR)/game/game.c -o $(OBJ_DIR)/game.o

clean:
	rm $(OBJ_DIR)/*.o
