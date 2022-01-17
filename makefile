SRC_DIR := src
OBJ_DIR := object

CXXFLAGS := -Wall -Wno-writable-strings -std=c++11
LDLIBS	 := -I/usr/local/include/SDL2 -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

.PHONY: clean

main: game.o platform.o
	g++ $(CXXFLAGS) $(LDLIBS) $(OBJ_DIR)/game.o $(OBJ_DIR)/platform.o -o main

platform.o:
	g++ $(CXXFLAGS) -c $(SRC_DIR)/platform.c -o $(OBJ_DIR)/platform.o

game.o:
	g++ $(CXXFLAGS) -c $(SRC_DIR)/game.c -o $(OBJ_DIR)/game.o

clean:
	rm $(OBJ_DIR)/*.o
