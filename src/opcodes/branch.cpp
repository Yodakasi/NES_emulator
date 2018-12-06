#include "../cpu.h"

void Cpu::BPL() {
    if(!getFlag(Negative)) {
        uint16_t old_PC = PC_reg;
        int8_t value = (int8_t)readFromMem(old_PC + 1);
        PC_reg += value + 2;
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
    if(getFlag(Negative)) {
        uint16_t old_PC = PC_reg;
        int8_t value = (int8_t)readFromMem(old_PC + 1);
        PC_reg += value + 2;
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

void Cpu::BVC() {
    if(!getFlag(Overflow)) {
        uint16_t old_PC = PC_reg;
        int8_t value = (int8_t)readFromMem(old_PC + 1);
        PC_reg += value + 2;
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


//Jumps


void Cpu::JMP(int addressingMode) {
    uint16_t value;
    switch(addressingMode) {
        case Absolute:
            value = readFromMem(PC_reg+1) + readFromMem(PC_reg+2) * 256;
            cycles += 3;
            break;
    }
    PC_reg = value;
}

void Cpu::JSR() {
    push((PC_reg + 2) >> 8);
    push((PC_reg + 2) & 0x00ff);
    PC_reg = absoluteIndexed(readFromMem(PC_reg + 1), readFromMem(PC_reg + 2), 0, false);
    cycles += 6;
}

void Cpu::RTI() {
    P_reg = pop();
    PC_reg = pop() + pop() * 256;
    cycles += 6;
}

void Cpu::RTS() {
    PC_reg = pop() + pop() * 256;
    PC_reg++;
    cycles += 6;
}