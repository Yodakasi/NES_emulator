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
