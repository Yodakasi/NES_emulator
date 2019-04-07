#include <iostream>
#include <cstdint>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "nes.h"


int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " <rom name in roms directory>" << std::endl;
        exit(0);
    }
    Nes nes(argv[1]);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event e;
    SDL_CreateWindowAndRenderer(256, 240, 0, &window, &renderer);
    std::cout << "Key mapping:" << std::endl << "x -> A | c -> B | s -> start | d -> select | arrow keys control movement" << std::endl;
    while(nes.isRunning()) {
        nes.run(renderer);
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT) {
            SDL_Log("Program quit after %i ticks", e.quit.timestamp);
            break;
        }

    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}