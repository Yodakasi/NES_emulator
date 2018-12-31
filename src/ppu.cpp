#include "ppu.h"


Ppu::Ppu(Cpu &cpu) {
    ZeroMem();
    for(size_t i=0; i<sizeof(registers)/sizeof(registers[0]); i++) {
        registers[i] = cpu.getIORegisterPointer(0x2000 + i);
    }
    addressToWriteIterator = 0;
    inNMI = false;
}

void Ppu::writeOamData(uint8_t *dataStart, uint8_t *dataEnd) {
    std::copy(dataStart, dataEnd, &memory[0x2000]);
    std::copy(dataStart, dataEnd, &memory[0x3000]);
}

void Ppu::dumpMem() const {
    std::cout << "Memory: " << std::endl;
    for(uint16_t i=0; i<0x4000; i++) {
        if(i % 0x100 == 0) {
            std::cout << std::endl << i << ": ";
        }
        std::cout << (int)memory[i] << " ";
    }
    std::cout << std::endl;
}

void Ppu::ZeroMem() {
    for(auto &i : memory)
        i = 0;
}

void Ppu::writeToMem(uint16_t address, uint8_t data) {
    address = address % 0x4000;
    memory[address] = data;
    if(address >= 0x2000 && address < 0x3000) {
        memory[address + 0x1000] = data;
    }
    else if(address >= 0x3f00 && address < 0x3f20) {
        for(int i=1; i<=7; i++) {
            memory[address+(i*0x20)] = data;
        }
    }
} //didnt check yet

uint8_t Ppu::readFromMem(uint16_t address) {
    return memory[address % 0x4000];
}

void Ppu::communicateWithCpu(Cpu &cpu) {
    if(cpu.newVal) {
        if(cpu.regAddr == 6) {
            if(addressToWriteIterator == 0) {
                addressToWrite = *registers[6] * 0x100;
                addressToWriteIterator++;
            }
            else if(addressToWriteIterator == 1) {
                addressToWrite += *registers[6];
                addressToWriteIterator++;
            }
            else if(addressToWriteIterator == 2) {
                addressToWrite = *registers[6] * 0x100;
                addressToWriteIterator = 1;
                *registers[7] = readFromMem(addressToWrite);
            }
        }
        else if(cpu.regAddr == 7) {
            writeToMem(addressToWrite, *registers[7]);
            (*registers[7])++;
        }
        cpu.newVal = false;
    }
    if((cpu.cycles >= 27280) && (cpu.cycles < 29781) && !inNMI) {
        *registers[0] |= 0x80;
        //if((*registers[0] & 0x80) == 0x80)
        *registers[2] |= 0x80;
        std::cout << "NMI!!!" << std::endl;
        inNMI = true;
    }
    else if (cpu.cycles >= 29781 && inNMI) {
        *registers[2] &= 0x7f;
        cpu.cycles = 0;
        inNMI = false;
        std::cout << "stop NMI!!!" << std::endl;
    }
}