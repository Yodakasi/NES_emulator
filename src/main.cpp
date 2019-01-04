#include <iostream>
#include <cstdint>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "nes.h"


int main() {
    Nes nes;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event e;
    SDL_CreateWindowAndRenderer(256, 240, 0, &window, &renderer);
    int i = 0;
    while(nes.isRunning()) {
        i++;
        nes.run(renderer);
        //if(i>200000) break;
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT) {
            SDL_Log("Program quit after %i ticks", e.quit.timestamp);
            break;
        }

    }
    nes.dump();
    //for(int i=0; i<=440; i++)
    //    nes.testPpu(renderer);


    
    sleep(10);
    SDL_DestroyWindow(window);
    SDL_Quit();
    /*
    cpu.dumpReg();
    cpu.dumpStack();
    //cpu.fetchOpcode();
    cpu.dumpMem();
    //cpu.dumpReg();
    */
}