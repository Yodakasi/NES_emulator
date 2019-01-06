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
    uint16_t tmp = A_reg + value + getFlag(Carry);
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

void Cpu::SBC(int addressingMode) {
    uint8_t value;
    //setFlag(1, Carry);
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
    value = value ^ 0xff;
    uint16_t tmp = A_reg + value + getFlag(Carry);
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

void Cpu::DECINC(int addressingMode, bool add) {
    uint8_t value;
    switch(addressingMode) {
        case ZeroPage:
            value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), 0));
            break;
        case ZeroPageIndexed:
            value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), X_reg));
            break;
        case Absolute:
            value = readFromMem(readFromMem(PC_reg+1) + readFromMem(PC_reg+2)*256);
            break;
        case AbsoluteIndexedX:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, false));
            break;
    }
    if(add)
        value++;
    else
        value--;
    if(value > 0x7f)
        setFlag(1, Negative);
    else
        setFlag(0, Negative);
    if(value == 0)
        setFlag(1, Zero);
    else
        setFlag(0, Zero);
    switch(addressingMode) {
        case ZeroPage:
            writeToMem(zeroPageIndexed(readFromMem(PC_reg+1), 0), value);
            PC_reg += 2;
            cycles += 5;
            break;
        case ZeroPageIndexed:
            writeToMem(zeroPageIndexed(readFromMem(PC_reg+1), X_reg), value);
            PC_reg += 2;
            cycles += 6;
            break;
        case Absolute:
            writeToMem(readFromMem(PC_reg+1) + readFromMem(PC_reg+2)*256, value);
            PC_reg += 3;
            cycles += 3;
            break;
        case AbsoluteIndexedX:
            writeToMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, false), value);
            PC_reg += 3;
            cycles += 7;
            break;
    }
}

void Cpu::DEX() {
    X_reg--;
    PC_reg++;
    cycles += 2;
    if(X_reg > 0x7f)
        setFlag(1, Negative);
    else
        setFlag(0, Negative);
    if(X_reg == 0)
        setFlag(1, Zero);
    else
        setFlag(0, Zero);
}

void Cpu::DEY() {
    Y_reg--;
    PC_reg++;
    cycles += 2;
    if(Y_reg > 0x7f)
        setFlag(1, Negative);
    else
        setFlag(0, Negative);
    if(Y_reg == 0)
        setFlag(1, Zero);
    else
        setFlag(0, Zero);
}

void Cpu::INY() {
    Y_reg++;
    PC_reg++;
    cycles += 2;
    if(Y_reg > 0x7f)
        setFlag(1, Negative);
    else
        setFlag(0, Negative);
    if(Y_reg == 0)
        setFlag(1, Zero);
    else
        setFlag(0, Zero);
}

void Cpu::INX() {
    X_reg++;
    PC_reg++;
    cycles += 2;
    if(X_reg > 0x7f)
        setFlag(1, Negative);
    else
        setFlag(0, Negative);
    if(X_reg == 0)
        setFlag(1, Zero);
    else
        setFlag(0, Zero);   
}