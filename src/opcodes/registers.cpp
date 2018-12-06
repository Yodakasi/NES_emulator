#include "../cpu.h"

void Cpu::CLC() {
    setFlag(0, Carry);
    PC_reg++;
    cycles += 2;
}

void Cpu::SEC() {
    setFlag(1, Carry);
    PC_reg++;
    cycles += 2;
}

void Cpu::CLI() {
    setFlag(0, Interupt);
    PC_reg++;
    cycles += 2;
}

void Cpu::SEI() {
    setFlag(1, Interupt);
    PC_reg++;
    cycles += 2;
}