SRC_DIR := src
OBJ_DIR := object

INCLIBS  := -Isrc/include -I/usr/local/include/SDL2
LDLIBS	 := -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

.PHONY: clean

main: game.o
	gcc $(INCLIBS) $(SRC_DIR)/platform/platform.c $(OBJ_DIR)/game.o -o main $(LDLIBS) 

game.o:
	gcc -c $(INCLIBS) $(SRC_DIR)/game/game.c -o $(OBJ_DIR)/game.o

clean:
	rm $(OBJ_DIR)/*.o
