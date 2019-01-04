#pragma once
#include <cstdint>
#include <memory>
#include <iostream>
#include <algorithm>
#include "cpu.h"
#include <SDL2/SDL.h>

class Ppu {
    uint8_t *registers[8];
    uint8_t memory[0x4000];
    uint8_t OAMMemory[0x100];
    uint16_t addressToWrite;
    uint8_t lastValue;
    int addressToWriteIterator;
    unsigned int scanline;
    unsigned int cycles;
    uint8_t imagePallete[4][4];
    uint8_t spritePallete[4][4];
    void writeToMem(uint16_t address, uint8_t data);
    inline uint8_t readFromMem(uint16_t address);
    void setPalletes();
    bool inNMI;
public:
    Ppu(Cpu &cpu);
    void writeOamData(uint8_t *dataStart, uint8_t *dataEnd);
    void communicateWithCpu(Cpu &cpu);
    void dumpMem() const;
    uint8_t *getChrRam() {return &memory[0];};
    void ZeroMem();
    void renderScanline(SDL_Renderer *renderer);
    unsigned int getCycles() {return cycles;}
};