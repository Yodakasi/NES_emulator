#include "../cpu.h"

void Cpu::setFlagOpcode(int value, int flag) {
    setFlag(value, flag);
    PC_reg++;
    cycles += 2;
}

void Cpu::compare(int addressingMode, uint8_t regValue) {
    uint8_t memValue;
    switch(addressingMode) {
        case ZeroPage:
            memValue = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), 0));
            PC_reg += 2;
            cycles += 3;
            break;
        case ZeroPageIndexed:
            memValue = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), X_reg));
            PC_reg += 2;
            cycles += 4;
            break;
        case Absolute:
            memValue = readFromMem(readFromMem(PC_reg+1) + readFromMem(PC_reg+2)*256);
            PC_reg += 3;
            cycles += 4;
            break;
        case AbsoluteIndexedY:
            memValue = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), Y_reg, true));
            PC_reg += 3;
            cycles += 4;
            break;
        case AbsoluteIndexedX:
            memValue = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, true));
            PC_reg += 3;
            cycles += 4;
            break;
        case IndexedIndirect:
            memValue = readFromMem(indexedIndirect(readFromMem(PC_reg+1)));
            PC_reg += 2;
            cycles += 6;
            break;
        case IndirectIndexed:
            memValue = readFromMem(indirectIndexed(readFromMem(PC_reg+1)));
            PC_reg += 2;
            cycles += 5;
            break;
        case Immediate:
            memValue = readFromMem(PC_reg+1);
            PC_reg += 2;
            cycles += 2;
            break;

    }
    uint8_t result = regValue - memValue;
    if(result)
        setFlag(0, Zero);
    else
        setFlag(1, Zero);
    if(result > regValue)
        setFlag(0, Carry);
    else
        setFlag(1, Carry);
    setFlag((result & 0x80) >> 7, Negative);
}