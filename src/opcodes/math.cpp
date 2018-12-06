#include "../cpu.h"

void Cpu::ADC(int addressingMode) {
    uint8_t value;
    switch(addressingMode) {
        case ZeroPage:
            value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), 0));
            cycles += 3;
            PC_reg += 2;
            break;
        case ZeroPageIndexed:
            value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), X_reg));
            cycles += 4;
            PC_reg += 2;
            break;
        case Absolute:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), 0, false));
            cycles += 4;
            PC_reg += 3;
            break;
        case AbsoluteIndexedX:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, true));
            cycles += 4;
            PC_reg += 3;
            break;
        case AbsoluteIndexedY:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), Y_reg, true));
            cycles += 4;
            PC_reg += 3;
            break;
        case IndexedIndirect:
            value = readFromMem(indexedIndirect(readFromMem(PC_reg+1)));
            cycles += 6;
            PC_reg += 2;
            break;
        case IndirectIndexed:
            value = readFromMem(indirectIndexed(readFromMem(PC_reg+1)));
            cycles += 5;
            PC_reg += 2;
            break;
        case Immediate:
            value = readFromMem(PC_reg+1);
            cycles += 2;
            PC_reg += 2;
            break;
    }
    uint16_t tmp = value + A_reg + getFlag(Carry);
    uint8_t result = tmp;
    setFlag((tmp & 256) >> 8, Carry);
    if((A_reg ^ result) & (value ^ result) & 0x80)
        setFlag(1, Overflow);
    else
        setFlag(0, Overflow);
    if(result > 0x7f)
        setFlag(1, Negative);
    else
        setFlag(0, Negative);
    if(result == 0)
        setFlag(1, Zero);
    else
        setFlag(0, Zero);
    A_reg = result;

}