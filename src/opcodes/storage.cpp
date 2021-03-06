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
        case Absolute:
            writeToMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), 0, false), value);
            PC_reg += 3;
            cycles += 4;
            break;
        case AbsoluteIndexedX:
            writeToMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, true), value);
            cycles += 4;
            PC_reg += 3;
            break;
        case AbsoluteIndexedY:
            writeToMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), Y_reg, true), value);
            cycles += 4;
            PC_reg += 3;
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

void Cpu::transfer(uint8_t from, int to) {
    if(to != SP) {
        if(from > 0x7f)
            setFlag(1, Negative);
        else
            setFlag(0, Negative);
        if(from == 0)
            setFlag(1, Zero);
        else
            setFlag(0, Zero);
    }
    switch(to) {
        case A:
            A_reg = from;
            break;
        case X:
            X_reg = from;
            break;
        case Y:
            Y_reg = from;
            break;
        case SP:
            SP_reg = from;
            break;
    }
    PC_reg++;
    cycles += 2;
}

void Cpu::load(int addressingMode, int regNum) {
    uint8_t value;
    switch(addressingMode) {
        case ZeroPage:
            value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), 0));
            PC_reg += 2;
            cycles += 3;
            break;
        case ZeroPageIndexed:
            if(regNum == X)
                value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), Y_reg));
            else
                value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), X_reg));
            PC_reg += 2;
            cycles += 4;
            break;
        case Absolute:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), 0, false));
            PC_reg += 3;
            cycles += 4;
            break;
        case AbsoluteIndexedX:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, true));
            PC_reg += 3;
            cycles += 4;
            break;
        case AbsoluteIndexedY:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), Y_reg, true));
            PC_reg += 3;
            cycles += 4;
            break;
        case IndexedIndirect:
            value = readFromMem(indexedIndirect(readFromMem(PC_reg+1)));
            PC_reg += 2;
            cycles += 6;
            break;
        case IndirectIndexed:
            value = readFromMem(indirectIndexed(readFromMem(PC_reg+1)));
            PC_reg += 2;
            cycles += 5;
            break;
        case Immediate:
            value = readFromMem(PC_reg+1);
            PC_reg += 2;
            cycles += 2;
            break;
    }
    if(value > 0x7f)
        setFlag(1, Negative);
    else
        setFlag(0, Negative);
    if(value == 0)
        setFlag(1, Zero);
    else
        setFlag(0, Zero);
    switch(regNum) {
        case A:
            if(!dontOverwriteA)
                A_reg = value;
            else
                dontOverwriteA = false;
            break;
        case X:
            X_reg = value;
            break;
        case Y:
            Y_reg = value;
            break;
    }
}