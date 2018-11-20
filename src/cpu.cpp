#include "cpu.h"

Cpu::Cpu() {
    cycles = 0;
    is_cpu_working = true;
    A_reg = 0;
    X_reg = 0;
    Y_reg = 0;
    SP_reg = 0xfd;
    P_reg = 0x34;
    ZeroMem();
    uploadRom();
    PC_reg = readFromMem(0xfffd) * 256 + readFromMem(0xfffc);
}

void Cpu::uploadRom() {
    writeToMem(0xfffd, 0x80);
    writeToMem(0xfffc, 0);
    writeToMem(0, 0x10);
    writeToMem(0x10, 0x50);
    writeToMem(0x8000, 1);
    writeToMem(0x8001, 0);
}

void Cpu::ZeroMem() {
    for(auto &i : memory)
        i = 0;
}

void Cpu::dumpMem() const {
    for(auto i : memory)
        std::cout << (int)i << " ";
    std::cout << std::endl;
}

void Cpu::dumpReg() const {
    std::cout << std::hex;
    std::cout << "A reg: "  << (int)A_reg << std::endl;
    std::cout << "X reg: "  << (int)X_reg << std::endl;
    std::cout << "Y reg: "  << (int)Y_reg << std::endl;
    std::cout << "PC reg: " << (int)PC_reg << std::endl;
    std::cout << "SP reg: " << (int)SP_reg << std::endl;
    std::cout << "P reg: "  << (int)P_reg << std::endl;
}

uint8_t Cpu::readFromMem(uint16_t address) const {

    return memory[address];
}

void Cpu::writeToMem(uint16_t address, uint8_t value) {
    memory[address] = value;
}


void Cpu::push(uint8_t value) {
    memory[SP_reg + 0x100] = value;
    SP_reg--;
}

uint8_t Cpu::pop() {
    uint8_t value = memory[SP_reg + 0x100];
    SP_reg++;
    return value;
}

void Cpu::setFlag(uint8_t value, uint8_t n) {
    if(value == 1) {
        P_reg |= (1 << n);
    }
    else if(value == 0) {
        P_reg &= ~(1 << n);
    }
}

uint16_t Cpu::zeroPageIndexed(uint16_t arg, uint16_t offset) const {
    return (arg + offset) % 256;
}

uint16_t Cpu::absoluteIndexed(uint16_t arg, uint16_t offset) const {
    return arg + offset;
}

uint16_t Cpu::indexedIndirect(uint16_t arg) const {
    return readFromMem((arg + X_reg) % 256) + readFromMem((arg + X_reg + 1) % 256) * 256;
}
uint16_t Cpu::indirectIndexed(uint16_t arg) const {
    return readFromMem(arg) + readFromMem((arg + 1) % 256) * 256 + Y_reg;
}

