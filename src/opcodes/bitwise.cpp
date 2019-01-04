#include "../cpu.h"

void Cpu::ORA(int addressingMode) {
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
    uint8_t result = value | A_reg;

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

void Cpu::AND(int addressingMode) {
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
        case AbsoluteIndexedY:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), Y_reg, true));
            cycles += 4;
            PC_reg += 3;
            break;
        case AbsoluteIndexedX:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, true));
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
    uint8_t result = value & A_reg;

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
//not sure if the result should be in accumulator also
void Cpu::ASL(int addressingMode) {
    uint8_t value;
    switch(addressingMode) {
        case ZeroPage:
            value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), 0));
            break;
        case ZeroPageIndexed:
            value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), X_reg));
            break;
        case Absolute:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), 0, false));
            break;
        case AbsoluteIndexedX:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, false));
            break;
        case Accumulator:
            value = A_reg;
            break;    
    }
    setFlag((value & 128) >> 7, Carry);
    uint8_t result = value << 1;
    if(result > 0x7f)
        setFlag(1, Negative);
    else
        setFlag(0, Negative);
    if(result == 0)
        setFlag(1, Zero);
    else
        setFlag(0, Zero);
    switch(addressingMode) {
        case ZeroPage:
            writeToMem(zeroPageIndexed(readFromMem(PC_reg+1), 0), result);
            PC_reg += 2;
            cycles += 5;
            break;
        case ZeroPageIndexed:
            writeToMem(zeroPageIndexed(readFromMem(PC_reg+1), X_reg), result);
            cycles += 6;
            PC_reg += 2;
            break;
        case Absolute:
            writeToMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), 0, false), result);
            cycles += 6;
            PC_reg += 3;
            break;
        case AbsoluteIndexedX:
            writeToMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, false), result);
            cycles += 7;
            PC_reg += 3;
            break;
        case Accumulator:
            //A_reg = result;
            PC_reg += 1;
            cycles += 2;
            break;
    }
    A_reg = result; // no information
    
}

void Cpu::ROL(int addressingMode) {
    uint8_t value;
    switch(addressingMode) {
        case ZeroPage:
            value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), 0));
            break;
        case ZeroPageIndexed:
            value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), X_reg));
            break;
        case Absolute:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), 0, false));
            break;
        case Accumulator:
            value = A_reg;
            break;
        case AbsoluteIndexedX:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, false));
            break;

    }
    uint8_t result = value << 1;
    if(getFlag(Carry)) 
        result |= 1;
    setFlag((value & 128) >> 7, Carry);
    if(result > 0x7f)
        setFlag(1, Negative);
    else
        setFlag(0, Negative);
    if(result == 0)
        setFlag(1, Zero);
    else
        setFlag(0, Zero);
    switch(addressingMode) {
        case ZeroPage:
            writeToMem(zeroPageIndexed(readFromMem(PC_reg+1), 0), result);
            PC_reg += 2;
            cycles += 5;
            break;
        case ZeroPageIndexed:
            writeToMem(zeroPageIndexed(readFromMem(PC_reg+1), X_reg), result);
            PC_reg += 2;
            cycles += 6;
            break;
        case Absolute:
            writeToMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), 0, false), result);
            PC_reg += 3;
            cycles += 6;
            break;
        case AbsoluteIndexedX:
            writeToMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, false), result);
            PC_reg += 3;
            cycles += 7;
            break;
        case Accumulator:
            //A_reg = result;
            PC_reg++;
            cycles += 2;
            break;
    }
    A_reg = result; // no information

    
}

void Cpu::ROR(int addressingMode) {
    uint8_t value;
    switch(addressingMode) {
        case ZeroPage:
            value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), 0));
            break;
        case ZeroPageIndexed:
            value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), X_reg));
            break;
        case Absolute:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), 0, false));
            break;
        case AbsoluteIndexedX:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, false));
            break;
        case Accumulator:
            value = A_reg;
            break;
    }
    uint8_t result = value >> 1;
    if(getFlag(Carry)) 
        result |= 128;
    setFlag(value & 1, Carry);
    if(result > 0x7f)
        setFlag(1, Negative);
    else
        setFlag(0, Negative);
    if(result == 0)
        setFlag(1, Zero);
    else
        setFlag(0, Zero);
    switch(addressingMode) {
        case ZeroPage:
            writeToMem(zeroPageIndexed(readFromMem(PC_reg+1), 0), result);
            PC_reg += 2;
            cycles += 5;
            break;
        case ZeroPageIndexed:
            writeToMem(zeroPageIndexed(readFromMem(PC_reg+1), X_reg), result);
            PC_reg += 2;
            cycles += 6;
            break;
        case Absolute:
            writeToMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), 0, false), result);
            PC_reg += 3;
            cycles += 6;
            break;
        case AbsoluteIndexedX:
            writeToMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, false), result);
            cycles += 7;
            PC_reg += 3;
            break;
        case Accumulator:
            //A_reg = result;
            PC_reg++;
            cycles += 2;
            break;
    }
    A_reg = result; // no information
}

void Cpu::EOR(int addressingMode) {
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
            value = readFromMem(readFromMem(PC_reg+1) + readFromMem(PC_reg+2)*256);
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
    uint8_t result = value ^ A_reg;
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

void Cpu::BIT(int addressingMode) {
    uint8_t value;
    switch(addressingMode) {
        case ZeroPage:
            value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), 0));
            cycles += 3;
            PC_reg += 2;
            break;
        case Absolute:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), 0, false));
            cycles += 4;
            PC_reg += 3;
            break;
    }
    setFlag((value & 128) >> 7, Negative);
    setFlag((value & 64) >> 6, Overflow);
    if((value & A_reg) > 0)
        setFlag(0, Zero);
    else
        setFlag(1, Zero);
}

void Cpu::LSR(int addressingMode) {
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
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, true));
            break;
        case Accumulator:
            value = A_reg;
            break;
    }
    setFlag(value & 1, Carry);
    uint8_t result = value >> 1;
    if(result)
        setFlag(0, Zero);
    else
        setFlag(1, Zero);
    switch(addressingMode) {
        case ZeroPage:
            writeToMem(zeroPageIndexed(readFromMem(PC_reg+1), 0), result);
            cycles += 5;
            PC_reg += 2;
            break;
        case ZeroPageIndexed:
            writeToMem(zeroPageIndexed(readFromMem(PC_reg+1), X_reg), result);
            cycles += 6;
            PC_reg += 2;
            break;
        case Absolute:
            writeToMem(readFromMem(PC_reg+1) + readFromMem(PC_reg+2)*256, result);
            cycles += 6;
            PC_reg += 3;
            break;
        case AbsoluteIndexedX:
            writeToMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), X_reg, true), result);
            cycles += 7;
            PC_reg += 3;
            break;
        case Accumulator:
            //A_reg = result;
            cycles += 2;
            PC_reg++;
            break;
    }
    A_reg = result; // no information
}