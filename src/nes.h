#pragma once
#include "cpu.h"
#include "ppu.h"
#include <SDL2/SDL.h>

class Nes {
    Cpu cpu;
    Ppu ppu;
    unsigned int prevCycles;
public:
    Nes() : cpu(), ppu(cpu) {uploadRom(); cpu.setPCReg(); prevCycles = 0;};
    void run(SDL_Renderer *renderer);
    void dump() const;
    bool isRunning() const;
    void uploadRom();

    void testPpu(SDL_Renderer *renderer);
};