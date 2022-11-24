#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, 
                  "Hello World",
                  "You have successfully compiled and linked an SDL2"
                  " program, congratulations.", NULL);

    return 0;
}