#include "nes.h"

void Nes::run() {
    cpu.fetchOpcode();
    if(cpu.getDmaFlag()) {
        ppu.writeOamData(cpu.dmaBegin(), cpu.dmaBegin()+0xff);
    }
    if(cpu.getNMIFlag()) {
        cpu.handleNMIInterupt();
    }
    ppu.communicateWithCpu(cpu);
}

bool Nes::isRunning() const {
    return cpu.getCpuState();
}

void Nes::dump() const {
    cpu.dumpReg();
    cpu.dumpStack();
    cpu.dumpMem();
    std::cout << std::endl << "PPU mem: " << std::endl;
    ppu.dumpMem();
}

void Nes::uploadRom() {
    std::ifstream romFile ("../roms/helloworld.nes", std::ifstream::binary);
    romFile.seekg(0x4);
    uint8_t pkgSize = romFile.peek();
    romFile.seekg(0x5);
    uint8_t chrSize = romFile.peek();
    romFile.seekg(0x6);
    uint8_t mapper = romFile.peek() >> 5;
    std::cout << (int)mapper << std::endl;
    if(romFile.good()) {
        if(mapper == 0) {
            std::cout << "mapper nr 0" << std::endl;
            if(pkgSize == 1) {
                romFile.seekg(0x10);
                romFile.read((char *)cpu.getFirstInsSeg(), 0x4000);
                romFile.seekg(0x10);
                romFile.read((char *)cpu.getSecondInsSeg(), 0x4000);
                if(chrSize == 1) {
                    romFile.seekg(0x4010);
                    romFile.read((char *)ppu.getChrRam(), 0x2000);
                }
            }
            else if(pkgSize == 2) {
                romFile.seekg(0x10);
                romFile.read((char *)cpu.getFirstInsSeg(), 0x4000);
                romFile.seekg(0x4010);
                romFile.read((char *)cpu.getSecondInsSeg(), 0x4000);
                if(chrSize == 1) {
                    romFile.seekg(0x8010);
                    romFile.read((char *)ppu.getChrRam(), 0x2000);
                }
            }
            
        }
        else if(mapper == 1) {
            std::cout << "mapper nr 1" << std::endl;
            if(pkgSize == 1) {
                romFile.seekg(0x10);
                romFile.read((char *)cpu.getFirstInsSeg(), 0x4000);
                romFile.seekg(0x10);
                romFile.read((char *)cpu.getSecondInsSeg(), 0x4000);
                if(chrSize == 1) {
                    romFile.seekg(0x4010);
                    romFile.read((char *)ppu.getChrRam(), 0x2000);
                }
            }
            else if(pkgSize == 2) {
                romFile.seekg(0x10);
                romFile.read((char *)cpu.getFirstInsSeg(), 0x4000);
                romFile.seekg(0x4010);
                romFile.read((char *)cpu.getSecondInsSeg(), 0x4000);
                if(chrSize == 1) {
                    romFile.seekg(0x8010);
                    romFile.read((char *)ppu.getChrRam(), 0x2000);
                }
            }
        }
        romFile.close();
    }
    
}