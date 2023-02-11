SRC_DIR := src
OBJ_DIR := object

LDLIBS	 := -I/usr/local/include/SDL2 -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

.PHONY: clean

main:
	gcc $(LDLIBS) $(SRC_DIR)/platform.c -o main

game.o:
	gcc $(CXXFLAGS) -c $(SRC_DIR)/game/game.c -o $(OBJ_DIR)/game.o

clean:
	rm $(OBJ_DIR)/*.o
