#ifndef __CPU_H__
#define __CPU_H__
#include <cstdint>
#include <iostream>

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
    mutable unsigned int cycles;

public:
    Cpu();
    enum flags {Carry, Zero, Interupt, Decimal, Break, Overflow = 6, Negative};
    enum adressingModes {ZeroPage, ZeroPageIndexed, Absolute, AbsoluteIndexedX, AbsoluteIndexedY, IndexedIndirect, IndirectIndexed, Immediate, Accumulator, Indirect};
    void uploadRom();
    void ZeroMem();
    void dumpMem() const;
    void dumpReg() const;
    void dumpStack() const;
    bool getCpuState() const {return is_cpu_working;}
    uint8_t readFromMem(uint16_t address) const;
    void writeToMem(uint16_t address, uint8_t value);
    void setFlag(uint8_t value, uint8_t n);
    bool getFlag(uint8_t n) const;
    uint16_t zeroPageIndexed(uint8_t arg, uint8_t offset) const;
    uint16_t absoluteIndexed(uint8_t arg1, uint8_t arg2, uint8_t offset, bool pageCrossing) const;
    uint16_t indirect(uint16_t arg) const;
    uint16_t indexedIndirect(uint8_t arg) const;
    uint16_t indirectIndexed(uint8_t arg) const;
    void push(uint8_t value);
    uint8_t pop();
    void fetchOpcode();
    void BRK();
    void ORA(int addressingMode);
    void ASL(int addressingMode);
    void PHP();
    void BPL();
    void CLC();
    void JSR();
    void AND(int addressingMode);
    void BIT(int addressingMode);
};

#endif