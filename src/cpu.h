#ifndef __CPU_H__
#define __CPU_H__
#include <cstdint>

class Cpu {
    bool is_cpu_working;
    uint16_t PC_reg;
    uint8_t SP_reg;
    uint8_t A_reg;
    uint8_t Y_reg;
    uint8_t X_reg;
    uint8_t P_reg;
    /*struct Memory {
        uint8_t ram[0xff];
        uint8_t stack[0xff];
        //uint8_t mirrors[0x5ff]; not sure if thats correct way
        uint8_t I_O_registers[0x7];
        //uint8_t mirrors[0x1ff7];
        uint8_t I_O_registers2[0x1ff7];
        uint8_t rom[0x1fdf];
        uint8_t sram[0x1fff];
        uint8_t prg_rom_lower[0x3fff];
        uint8_t prg_rom_upper[0x3fff];
    } memory;*/
    uint8_t memory[0xffff];

public:
    Cpu();
    bool getCpuState() const {return is_cpu_working;}
    uint8_t *zero_page_addressing(unsigned int offset);
    uint8_t *indexed_zero_page_addressing(unsigned int offset, char reg);
    uint8_t *absolete_addressing(unsigned int offset);
    uint8_t *indexed_absolete_addressing(unsigned int offset, char reg);
    uint8_t *indirect_addressing(unsigned int offset);
    uint8_t *indexed_indirect_addressing(unsigned int offset);
    uint8_t *indirect_indexed_addressing(unsigned int offset);
};

#endif