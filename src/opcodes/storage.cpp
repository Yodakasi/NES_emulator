#include "../cpu.h"

void Cpu::store(int addressingMode, uint8_t value, int index) {
    switch(addressingMode) {
        case ZeroPage:
            writeToMem(zeroPageIndexed(readFromMem(PC_reg+1), 0), value);
            cycles += 3;
            PC_reg += 2;
            break;
        case ZeroPageIndexed:
            writeToMem(zeroPageIndexed(readFromMem(PC_reg+1), index), value);
            cycles += 4;
            PC_reg += 2;
            break;
        case IndexedIndirect:
            writeToMem(indexedIndirect(readFromMem(PC_reg+1)), value);
            cycles += 6;
            PC_reg += 2;
            break;
        case IndirectIndexed:
            writeToMem(indirectIndexed(readFromMem(PC_reg+1)), value);
            cycles += 5;
            PC_reg += 2;
            break;
        
    }
}

void Cpu::transfer(uint8_t from, uint8_t to) {
    
}