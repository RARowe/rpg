SRC_DIR := src
OBJ_DIR := object

CXXFLAGS := -Wall -Wno-writable-strings -std=c++11 -fpermissive
LDLIBS	 := -I/usr/local/include/SDL2 -L. -L/usr/local/lib -L/lib/x86_64-linux-gnu -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

.PHONY: clean

main: libgame.so libplatform.so
	g++ $(CXXFLAGS) $(SRC_DIR)/main.c -o main $(LDLIBS) -lplatform

libplatform.so:
	g++ $(CXXFLAGS) -fPIC -shared $(SRC_DIR)/platform.c -o libplatform.so $(LDLIBS)

libgame.so: src/game.c
	g++ $(CXXFLAGS) -fPIC -shared $(SRC_DIR)/game.c -o libgame.so

clean:
	rm $(OBJ_DIR)/*.o
