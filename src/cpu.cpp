#include "cpu.h"
//maybe add aserts instead of returning nullptrt

Cpu::Cpu() {
    is_cpu_working = true;
    A_reg = 0;
    X_reg = 0;
    Y_reg = 0;
    P_reg = 0x34; //not sure if hex
}

uint8_t *Cpu::zero_page_addressing(unsigned int offset) {
    return &memory[offset % 0xff];
}

uint8_t *Cpu::indexed_zero_page_addressing(unsigned int offset, char reg) {
    if(reg == 'x') {
        return &memory[(offset + X_reg) % 0xff];
    }
    else if(reg == 'y') {
        return &memory[(offset + Y_reg) % 0xff];
    }
    return nullptr;
}

uint8_t *Cpu::absolete_addressing(unsigned int offset) {
    if(offset > 0xffff) {
        return nullptr;
    }
    return &memory[offset];
}

uint8_t *Cpu::indexed_absolete_addressing(unsigned int offset, char reg) {
    if(offset > 0xffff) {
        return nullptr;
    }
    if(reg == 'x') {
        return &memory[offset];
    }
    else if(reg == 'y') {
        return &memory[offset];
    }
    return nullptr;
}

uint8_t *Cpu::indirect_addressing(unsigned int offset) {
    if(offset > 0xffff) {
        return nullptr;
    }
    unsigned int new_offset = memory[offset] + memory[offset + 1] * 0x100;
    if(new_offset > 0xffff) {
        return nullptr;
    }
    return &memory[new_offset];
}

uint8_t *Cpu::indexed_indirect_addressing(unsigned int offset) {
    unsigned int new_offset = memory[(offset + X_reg) % 0xffff] + memory[(offset + X_reg + 1) % 0xffff] * 0x100;
    return &memory[new_offset % 0xffff]; //not sure where it wraparounds
}

uint8_t *Cpu::indirect_indexed_addressing(unsigned int offset) {
    unsigned int new_offset = memory[offset] + memory[offset + 1] * 0x100;
    return &memory[new_offset + Y_reg];
}