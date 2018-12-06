#include "../cpu.h"

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

void Cpu::PHA() {
    push(A_reg);
    PC_reg++;
    cycles += 3;
}

void Cpu::PLA() {
    A_reg = pop();
    if(A_reg > 0x7f)
        setFlag(1, Negative);
    else
        setFlag(0, Negative);
    if(A_reg == 0)
        setFlag(1, Zero);
    else
        setFlag(0, Zero);
    cycles += 4;
    PC_reg++;
}