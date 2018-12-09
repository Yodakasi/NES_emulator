#pragma once
#include <cstdint>
#include <memory>
#include <iostream>
#include <algorithm>
#include "cpu.h"

class Ppu {
    uint8_t *registers[8];
    uint8_t memory[0x4000];
    uint16_t addressToWrite;
    uint8_t lastValue;
    int addressToWriteIterator;
    void writeToMem(uint16_t address, uint8_t data);
public:
    Ppu(Cpu &cpu);
    void writeOamData(uint8_t *dataStart, uint8_t *dataEnd);
    void writeFromCpu();
    void dumpMem() const;
    void ZeroMem();
};