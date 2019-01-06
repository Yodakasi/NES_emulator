#pragma once
#include <cstdint>
#include <memory>
#include <iostream>
#include <algorithm>
#include "cpu.h"
#include <SDL2/SDL.h>

struct spriteInfo {
    uint8_t bankNum;
    uint8_t index;
    uint8_t palleteNum;
    bool flipHorizon;
    bool flipVert;
    bool inFront;
    uint8_t xPos;
};

class Ppu {
    uint8_t *registers[8];
    uint8_t memory[0x4000];
    uint8_t OAMMemory[0x100];
    uint16_t addressToWrite;
    uint16_t addressToRead;
    uint8_t lastValue;
    int addressToWriteIterator;
    uint8_t scrollx;
    uint8_t scrolly;
    uint8_t scrollIndex;
    unsigned int scanline;
    unsigned int cycles;
    uint8_t imagePallete[4][4];
    uint8_t spritePallete[4][4];
    bool inNMI;
    spriteInfo sprites[8];
    void writeToMem(uint16_t address, uint8_t data);
    inline uint8_t readFromMem(uint16_t address);
    void setPalletes();
    void fetchSprites();
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