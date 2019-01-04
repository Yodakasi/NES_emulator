#include "ppu.h"
#include "palleteColors.h"

Ppu::Ppu(Cpu &cpu) {
    ZeroMem();
    for(size_t i=0; i<sizeof(registers)/sizeof(registers[0]); i++) {
        registers[i] = cpu.getIORegisterPointer(0x2000 + i);
    }
    addressToWriteIterator = 0;
    inNMI = false;
    scanline = 0;
    cycles = 0;
}

void Ppu::writeOamData(uint8_t *dataStart, uint8_t *dataEnd) {
    std::copy(dataStart, dataEnd, &OAMMemory[0]);
    std::cout << "DMAAAAAAA" << std::endl;
    //std::copy(dataStart, dataEnd, &memory[0x3000]);
}

void Ppu::dumpMem() const {
    std::cout << "Memory: " << std::endl;
    for(uint16_t i=0; i<0x4000; i++) {
        if(i % 0x100 == 0) {
            std::cout << std::endl << i << ": ";
        }
        std::cout << (int)memory[i] << " ";
    }
    std::cout << std::endl << std::endl;
    std::cout << "OAM Memory" << std::endl;
    for(int i=0; i<0x100; i++) {
        std::cout << (int)OAMMemory[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Attribute table " << std::endl;
    for(int i=0; i<64; i++) {
        std::cout << (int)memory[i + 0x23c0] << " ";
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
}

inline uint8_t Ppu::readFromMem(uint16_t address) {
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
            addressToWrite++;
            std::cout << "no pisze co chcesz " << std::hex << (int)addressToWrite << " " << (int)*registers[7] << std::endl;
        }
        cpu.newVal = false;
    }
    /*
    if((cpu.cycles >= 27280) && (cpu.cycles < 29781) && !inNMI) {
        // *registers[0] |= 0x80;
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
    */
}

void Ppu::setPalletes() {
    for(int i=0; i<4; i++) {
        imagePallete[0][i] = readFromMem(0x3f00);
        spritePallete[0][i] = readFromMem(0x3f00);
    }
    for(int i=0; i<4; i++) {
        for(int j=1; j<4; j++) {
            imagePallete[j][i] = readFromMem(0x3f00 + j + i*4);
            spritePallete[j][i] = readFromMem(0x3f10 + j + i*4);
        }
    }
}

void Ppu::renderScanline(SDL_Renderer *renderer) {
    if(scanline < 240) {
        if(scanline == 0) {
            setPalletes();
            SDL_RenderClear(renderer);
        }
        int spriteSize ;
        //writeToMem(0x2000, 15);
        //writeToMem(0x2001, 14);
        //writeToMem(0x2021, 13);
        int offset;
        if(*registers[0] & 16)
            offset = 0x1000;
        else
            offset = 0;
        if(*registers[0] & 32) {
            spriteSize = 16;
            offset = 0;
        }
        else
            spriteSize = 8;
        for(int i=0; i<256; i++) {
            int palleteNumber;
            uint8_t address = readFromMem(0x2000 + (scanline/spriteSize)*32 + i/8);
            std::cout << " " << std::hex << (int)(0x2000 + (scanline/spriteSize)*32 + i/8);
            
            if((scanline/8) & 1) {
                if(i/16 & 1)
                    palleteNumber = readFromMem(0x23c0 + i/32 + (scanline/8)*8) & 3;
                else
                    palleteNumber = (readFromMem(0x23c0 + i/32 + (scanline/8)*8) >> 2) & 3;    
            }
            else {
                if(i/16 & 1)
                    palleteNumber = (readFromMem(0x23c0 + i/32 + (scanline/8)*8) >> 4) & 3;
                else
                    palleteNumber = readFromMem(0x23c0 + i/32 + (scanline/8)*8) >> 6;
            }
            int patterTablePart = (((readFromMem(offset + address*(spriteSize*2) + scanline % spriteSize) & (1 << (7 - (i % 8)))) >> (7 - (i % 8))) * 2) 
                                   + ((readFromMem(offset + address*(spriteSize*2) + scanline % spriteSize + spriteSize) & (1 << (7 - (i % 8)))) >> (7 - (i % 8)));
            //int patterTablePart = readFromMem(0x1000 + address*(spriteSize*2) + scanline % spriteSize)
            
            //std::cout << "first " << (int)readFromMem(0x1000 + address*(spriteSize*2) + scanline % spriteSize) << " second " << (int)readFromMem(0x1000 + address*(spriteSize*2) + scanline % spriteSize + spriteSize) << " " << (int)patterTablePart << " ";
            SDL_SetRenderDrawColor(renderer, palleteColors::pallete[imagePallete[patterTablePart][palleteNumber]].r,
                                             palleteColors::pallete[imagePallete[patterTablePart][palleteNumber]].g,
                                             palleteColors::pallete[imagePallete[patterTablePart][palleteNumber]].b, 255);
            SDL_RenderDrawPoint(renderer, i, scanline);
        }
        //for(int i=0; i<32; i++)
        //std::cout << (int)readFromMem(0x1000 + 0x10*(spriteSize*2) + scanline % spriteSize) << " ";
        std::cout << std::endl;
        
    }
    else if (scanline == 241) {
        SDL_RenderPresent(renderer);
        *registers[2] |= 0x80;
        //*registers[0] |= 0x80;
        std::cout << "NMI!!!" << std::endl;
    }
    else if(scanline == 260) {
        *registers[2] &= 0x7f;
        scanline = -1;
        std::cout << "stop NMI!!!" << std::endl;
    }
    scanline++;
    cycles += 341;
}