#include "cpu.h"

Cpu::Cpu() {
    cycles = 0;
    is_cpu_working = true;
    A_reg = 0;
    X_reg = 0;
    Y_reg = 0;
    SP_reg = 0xfd;
    P_reg = 0;
    ZeroMem();
    uploadRom();
    PC_reg = readFromMem(0xfffd) * 256 + readFromMem(0xfffc);
}

void Cpu::uploadRom() {
    std::ifstream romFile ("../roms/helloworld.nes", std::ifstream::binary);
    if(romFile.good()) {
        romFile.seekg(0x10);
        romFile.read((char *)&memory[0x8000], 0x4000);
        romFile.seekg(0x10);
        romFile.read((char *)&memory[0xC000], 0x4000);
        romFile.close();
    }
    cycles = 0;
    
}

void Cpu::ZeroMem() {
    for(auto &i : memory)
        i = 0;
}

void Cpu::dumpMem() const {
    std::cout << "Memory: " << std::endl;
    for(uint16_t i=0; i<0xffff; i++) {
        if(i % 0x100 == 0) {
            std::cout << std::endl << i << ": ";
        }
        std::cout << (int)memory[i] << " ";
    }
    std::cout << std::endl;
}

void Cpu::dumpStack() const {
    std::cout << "Stack: " << std::endl;
    for(uint16_t i=0x100; i<=0x1ff; i++) {
        std::cout << (int)memory[i] << " ";
    }
    std::cout << std::endl;
}

void Cpu::dumpReg() const {
    std::cout << std::hex;
    std::cout << "A reg: "  << (int)A_reg << std::endl;
    std::cout << "X reg: "  << (int)X_reg << std::endl;
    std::cout << "Y reg: "  << (int)Y_reg << std::endl;
    std::cout << "PC reg: " << (int)PC_reg << std::endl;
    std::cout << "SP reg: " << (int)SP_reg << std::endl;
    std::cout << "cycles: " << (int)cycles << std::endl;
    std::cout << "P reg " << (unsigned int)P_reg
              << " Carry: "  << (P_reg & 1)
              << " Zero: " << ((P_reg & 2) >> 1) 
              << " Interupt: " << ((P_reg & 4) >> 2) 
              << " Decimal: " << ((P_reg & 8) >> 3) 
              << " Overflow: " << ((P_reg & 64) >> 6) 
              << " Negative: " << ((P_reg & 128) >> 7) << std::endl;
}

uint8_t Cpu::readFromMem(uint16_t address) const {

    return memory[address];
}

void Cpu::writeToMem(uint16_t address, uint8_t value) {
    memory[address] = value;
    if(address < 0x800) {
        for(int i=1; i<4; i++) {
            memory[address+(i*0x800)] = value;
        }
    }
    else if(address >= 0x2000 && address <= 0x2007) {
        for(int i=1; i<0x3ff; i++) {
            memory[address+(i*8)] = value;
        }
    }
}


void Cpu::push(uint8_t value) {
    SP_reg--;
    writeToMem(SP_reg + 0x100, value);
}

uint8_t Cpu::pop() {
    uint8_t value = readFromMem(SP_reg + 0x100);
    SP_reg++;
    return value;
}

void Cpu::setFlag(uint8_t value, uint8_t n) {
    if(value == 1) {
        P_reg |= 1UL << n;
    }
    else if(value == 0) {
        P_reg &= ~(1UL << n);
    }
}

bool Cpu::getFlag(uint8_t n) const {
    return ((P_reg & ( 1 << n )) >> n) == 1;
}

uint16_t Cpu::zeroPageIndexed(uint8_t arg, uint8_t offset) const {
    return (arg + offset) % 256;
}

uint16_t Cpu::absoluteIndexed(uint8_t arg1, uint8_t arg2, uint8_t offset, bool pageCrossing) const {
    uint16_t address = arg1 + (arg2 % 256) * 256;
    if(offset != 0) {
        if(((address & 0xff00) != ((address+offset) & 0xff00)) && pageCrossing)
            cycles += 1;
        address += offset; 
    }
    return address;
}

uint16_t Cpu::indirect(uint16_t arg) const {
    return readFromMem(arg) + readFromMem(arg + 1) * 256;
}

uint16_t Cpu::indexedIndirect(uint8_t arg) const {
    return readFromMem((arg + X_reg) % 256) + readFromMem((arg + X_reg + 1) % 256) * 256;
}
uint16_t Cpu::indirectIndexed(uint8_t arg) const {
    uint16_t ret = readFromMem(arg) + readFromMem((arg + 1) % 256) * 256;
    if((ret & 0xff00)  != ((ret+Y_reg) & 0xff00))
        cycles += 1;
    return ret + Y_reg;
}
