#pragma once
#include "cpu.h"
#include "ppu.h"

class Nes {
    Cpu cpu;
    Ppu ppu;
public:
    Nes() : cpu(), ppu(cpu) {uploadRom(); cpu.setPCReg();};
    void run();
    void dump() const;
    bool isRunning() const;
    void uploadRom();
};