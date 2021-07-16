INC_DIR := headers
SRC_DIR := src
OBJ_DIR := object
LIB_DIR := $(shell pwd)/lib

CC		 := g++
CPPFLAGS := -I$(INC_DIR) -MMD -MP
CXXFLAGS := -Wall -std=c++17 -D_THREAD_SAFE -O3
LDLIBS	 := -I/usr/local/include/SDL2 -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf ./lib/libopendialog.dylib

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS	:= $(wildcard $(OBJ_DIR)/*.d)

.PHONY: clean

main: $(OBJECTS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

#$(OBJECTS): $(OBJ_DIR)/%.o: $(INC_DIR)/%.h $(SRC_DIR)/%.cpp
$(OBJECTS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $(OUTPUT_OPTION) $<

clean: ; $(RM) $(DEPS) $(OBJECTS)

open_dialog: FORCE
	gcc -Wall -D_THREAD_SAFE -O3 -MMD -MP -framework AppKit -dynamiclib -fpic -o ./lib/libopendialog.dylib ./open_dialog/open_dialog.m

FORCE:

include $(DEPS)

$(MAKEFILE_LIST): ;
%:: %,v
%:: RCS/%,v
%:: RCS/%
%:: s.%
%:: SCCS/s.%
