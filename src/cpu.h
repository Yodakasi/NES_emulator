#pragma once
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
    uint8_t memory[0xffff];
    mutable unsigned int cycles;
    enum flags {Carry, Zero, Interupt, Decimal, Break, Overflow = 6, Negative};
    enum adressingModes {ZeroPage, ZeroPageIndexed, Absolute, AbsoluteIndexedX, AbsoluteIndexedY, IndexedIndirect, IndirectIndexed, Immediate, Accumulator, Indirect};
    enum registers {A, X, Y, SP};

    //memory operations
    uint8_t readFromMem(uint16_t address) const;
    void writeToMem(uint16_t address, uint8_t value);

    //flag register operations
    void setFlag(uint8_t value, uint8_t n);
    bool getFlag(uint8_t n) const;

    //addresing
    uint16_t zeroPageIndexed(uint8_t arg, uint8_t offset) const;
    uint16_t absoluteIndexed(uint8_t arg1, uint8_t arg2, uint8_t offset, bool pageCrossing) const;
    uint16_t indirect(uint16_t arg) const;
    uint16_t indexedIndirect(uint8_t arg) const;
    uint16_t indirectIndexed(uint8_t arg) const;
    
    //stack operations
    void push(uint8_t value);
    uint8_t pop();


    //OPCODES

    //system
    void BRK();

    //math
    void ADC(int addressingMode);

    //bitwise
    void ORA(int addressingMode);
    void ASL(int addressingMode);
    void AND(int addressingMode);
    void BIT(int addressingMode);
    void ROL(int addressingMode);
    void EOR(int addressingMode);
    void ROR(int addressingMode);
    void LSR(int addressingMode);

    //branch
    void branch(int flag, int value);
    void JSR();
    void JMP(int addressingMode);
    void RTS();
    void RTI();

    //registers
    void CLI();
    void SEI();
    void CLC();
    void SEC();

    //stack
    void PHA();
    void PHP();
    void PLP();
    void PLA();
    
    //storage    
    void store(int addressingMode, uint8_t value, int index = 0);
    void transfer(uint8_t from, int to);
    void load(int addressingMode, int regNum);

public:
    Cpu();
    void fetchOpcode();
    void uploadRom();
    void ZeroMem();
    void dumpMem() const;
    void dumpReg() const;
    void dumpStack() const;
    bool getCpuState() const {return is_cpu_working;}
};
