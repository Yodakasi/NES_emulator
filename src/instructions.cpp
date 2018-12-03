#include "cpu.h"

void Cpu::fetchOpcode() {
    uint8_t opcode = readFromMem(PC_reg);
    std::cout << "Current opcode: " << (int)opcode << std::endl;
    switch(opcode) {
        case 0x00:
            BRK();
            break;
        case 0x1:
            ORA(IndexedIndirect);
            break;
        case 0x5:
            ORA(ZeroPage);
            break;
        case 0x6:
            ASL(ZeroPage);
            break;
        case 0x8:
            PHP();
            break;
        case 0x9:
            ORA(Immediate);
            break;
        case 0xa:
            ASL(Accumulator);
            break;
        case 0xd:
            ORA(Absolute);
            break;
        case 0xe:
            ASL(Absolute);
            break;
        case 0x10:
            BPL();
            break;
        case 0x11:
            ORA(IndirectIndexed);
            break;
        case 0x15:
            ORA(ZeroPageIndexed);
            break;
        case 0x16:
            ASL(ZeroPageIndexed);
            break;
        case 0x18:
            CLC();
            break;
        case 0x19:
            ORA(AbsoluteIndexedY);
            break;
        case 0x1d:
            ORA(AbsoluteIndexedX);
            break;
        case 0x1e:
            ASL(AbsoluteIndexedX);
            break;
        case 0x20:
            JSR();
            break;
        case 0x21:
            AND(IndexedIndirect);
            break;
        case 0x24:
            BIT(ZeroPage);
            break;
        case 0x25:
            AND(ZeroPage);
            break;
        case 0x26:
            ROL(ZeroPage);
            break;
        case 0x28:
            PLP();
            break;
        case 0x29:
            AND(Immediate);
            break;
        case 0x2a:
            ROL(Accumulator);
            break;
        case 0x2c:
            BIT(Absolute);
            break;
        case 0x2d:
            AND(Absolute);
            break;
        case 0x2e:
            ROL(Absolute);
            break;
        default:
            std::cout << "Instruction undefined" << std::endl;
    }
}

//System instructions

void Cpu::BRK() {
    setFlag(1, Interupt);
    push(PC_reg+2);
    push(SP_reg);
    PC_reg++;
    cycles += 7;
}

//Bitwise instructions

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
    std::cout << "walue: " << (int)value << " " << (int)A_reg << std::endl;
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
        case Absolute:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), 0, false));
            cycles += 4;
            PC_reg += 3;
            break;
        case IndexedIndirect:
            value = readFromMem(indexedIndirect(readFromMem(PC_reg+1)));
            cycles += 6;
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
            A_reg = result;
            PC_reg += 1;
            cycles += 2;
            break;
    }
    //A_reg = result; // no information
    
}

void Cpu::ROL(int addressingMode) {
    uint8_t value;
    switch(addressingMode) {
        case ZeroPage:
            value = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), 0));
            break;
        case Absolute:
            value = readFromMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), 0, false));
            break;
        case Accumulator:
            value = A_reg;
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
        case Absolute:
            writeToMem(absoluteIndexed(readFromMem(PC_reg+1), readFromMem(PC_reg+2), 0, false), result);
            PC_reg += 3;
            cycles += 6;
            break;
        case Accumulator:
            A_reg = result;
            PC_reg++;
            cycles += 2;
            break;
    }

    
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
    }
    setFlag((value & 128) >> 7, Negative);
    setFlag((value & 64) >> 6, Overflow);
    if((value & A_reg) > 0)
        setFlag(0, Zero);
    else
        setFlag(1, Zero);
}

//Stack

void Cpu::PHP() {
    push(P_reg);
    PC_reg++;
    cycles += 3;
}

void Cpu::PLP() {
    P_reg = pop();
    PC_reg++;
    cycles += 4;
}

//Branches

void Cpu::BPL() {
    if(!getFlag(Negative)) {
        uint16_t old_PC = PC_reg;
        PC_reg += readFromMem(old_PC + 1) + 2;
        if((PC_reg & 0xff00) != (old_PC & 0xff00))
            cycles += 2;
        else
            cycles += 1;
    }
    else {
        PC_reg += 2;
    }
    cycles += 2;
}

void Cpu::BMI() {
    
}


//registers

void Cpu::CLC() {
    setFlag(0, Carry);
    PC_reg++;
    cycles += 2;
}

//jumps

void Cpu::JSR() {
    std::cout << "elo " << (int)((PC_reg + 2) >> 8) << std::endl;
    push((PC_reg + 2) >> 8);
    push((PC_reg + 2) & 0x00ff);
    PC_reg = absoluteIndexed(readFromMem(PC_reg + 1), readFromMem(PC_reg + 2), 0, false);
    cycles += 6;
}