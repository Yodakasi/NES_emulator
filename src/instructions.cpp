#include "cpu.h"

void Cpu::fetchOpcode() {
    uint8_t opcode = readFromMem(PC_reg);
    std::cout << "Current opcode: " << (int)opcode << std::endl;
    switch(opcode) {
        case 0x00:
            BRK();
            std::cout << "BRK" << std::endl;
            break;
        case 0x1:
            ORAindexedIndirect();
            std::cout << "ORA" << std::endl;
            break;
        case 0x5:
            ORAZeroPage();
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

void Cpu::ORAindexedIndirect() {
    uint8_t result = readFromMem(indexedIndirect(readFromMem(PC_reg+1))) | A_reg;
    if(result > 0x7f)
        setFlag(1, Negative);
    else
        setFlag(0, Negative);
    if(result == 0)
        setFlag(1, Zero);
    else
        setFlag(0, Zero);
    A_reg = result;
    cycles += 6;
    PC_reg += 2;
}

void Cpu::ORAZeroPage() {
    uint8_t result = readFromMem(zeroPageIndexed(readFromMem(PC_reg+1), 0)) | A_reg;
    if(result > 0x7f)
        setFlag(1, Negative);
    else
        setFlag(0, Negative);
    if(result == 0)
        setFlag(1, Zero);
    else
        setFlag(0, Zero);
    A_reg = result;
    cycles += 6;
    PC_reg += 2;
}