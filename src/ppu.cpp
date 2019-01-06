#include "ppu.h"
#include "palleteColors.h"

Ppu::Ppu(Cpu &cpu) {
    ZeroMem();
    for(size_t i=0; i<sizeof(registers)/sizeof(registers[0]); i++) {
        registers[i] = cpu.getIORegisterPointer(0x2000 + i);
    }
    addressToWriteIterator = 0;
    scrollIndex = 0;
    scrollx = 0;
    scrolly = 0;
    inNMI = false;
    scanline = 0;
    cycles = 0;
}

void Ppu::writeOamData(uint8_t *dataStart, uint8_t *dataEnd) {
    std::copy(dataStart, dataEnd, &OAMMemory[0]);
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
    if(cpu.ReadFromPpuReg) {
        *registers[7] = readFromMem(addressToRead);
        if(*registers[0] & 4)
            addressToRead += 32;
        else
            addressToRead++;
        cpu.ReadFromPpuReg = false;
    }
    else if(cpu.newVal) {
        if(cpu.regAddr == 6) {
            if(*registers[6] == 0) {
                //addressToWrite = 0;
                addressToWriteIterator = 0;
            }
            else {
                if(addressToWriteIterator == 0) {
                    addressToWrite = *registers[6] * 0x100;
                    addressToRead = *registers[6] * 0x100;
                    addressToWriteIterator++;
                }
                else if(addressToWriteIterator == 1) {
                    addressToWrite += *registers[6];
                    addressToRead = *registers[6];
                    addressToWriteIterator = 0;
                }
                /*
                else if(addressToWriteIterator == 2) {
                    addressToWrite = *registers[6] * 0x100;
                    addressToWriteIterator = 1;
                    *registers[7] = readFromMem(addressToWrite);
                }*/
            }
        }
        else if(cpu.regAddr == 7) {
            writeToMem(addressToWrite, *registers[7]);
            if(addressToWrite == 0x3f10) {
                writeToMem(0x3f00, *registers[7]);
            }
            if(addressToWrite == 0x3f14) {
                writeToMem(0x3f04, *registers[7]);
            }
            if(addressToWrite == 0x3f18) {
                writeToMem(0x3f08, *registers[7]);
            }
            if(addressToWrite == 0x3f1c) {
                writeToMem(0x3f0c, *registers[7]);
            }
            if(*registers[0] & 4)
                addressToWrite += 32;
            else
                addressToWrite++;
            //std::cout << "no pisze co chcesz " << std::hex << (int)(addressToWrite % 0x4000) << " " << (int)*registers[7] << std::endl;
        }
        else if(cpu.regAddr == 4) {
            OAMMemory[*registers[3]] = *registers[4];
            *registers[3]++;
        }
        else if(cpu.regAddr == 5) {
            if(scrollIndex == 0) {
                scrollx = *registers[5];
                scrollIndex++;
            }
            else if(scrollIndex == 1) {
                scrolly = *registers[5];
                scrollIndex = 0;
            }
        }
        cpu.newVal = false;
    }
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

void Ppu::fetchSprites() {
    int j = 0;
    for(int i=0; i<8; i++) {
        while(scanline != OAMMemory[j*4]) {
            j++;
            if(j == 65)
                return;
        }
        sprites[i].bankNum = OAMMemory[j*4 + 1] & 1;
        if(*registers[0] & 32)
            sprites[i].bankNum = 0;
        sprites[i].index = OAMMemory[j*4 + 1] & 0xfe;
        sprites[i].palleteNum = OAMMemory[j*4 + 2] & 3;
        if(OAMMemory[j*4 + 2] & 32)
            sprites[i].inFront = true;
        else
            sprites[i].inFront = false;
        if(OAMMemory[j*4 + 2] & 64)
            sprites[i].flipHorizon = true;
        else
            sprites[i].flipHorizon = false;
        if(OAMMemory[j*4 + 2] & 128)
            sprites[i].flipVert = true;
        else
            sprites[i].flipVert = false;
        sprites[i].xPos = OAMMemory[j*4 + 3];
    }
}

void Ppu::renderScanline(SDL_Renderer *renderer) {
    if(scanline < 240) {
        if(scanline == 0) {
            setPalletes();
            SDL_RenderClear(renderer);
        }
        if(scanline % 8 == 0) {
            fetchSprites();
        }
        int spriteSize;
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
        //std::cout << (int)scrollx << " " << (int)scrolly;
        for(int i=0; i<256; i++) {
            int palleteNumber;
            uint8_t address = readFromMem(0x2000 + (scanline/spriteSize)*32 + i/8 + scrollx + scrolly*32);
            //std::cout << " " << std::hex << (int)(0x2000 + (scanline/spriteSize)*32 + i/8 + scrollx + scrolly*32);

            if((scanline/16) & 1) {
                if((i/8) & 1)
                    palleteNumber = (readFromMem(0x23c0 + i/32 + (scanline/32)*8) >> 4) & 3;
                else
                    palleteNumber = readFromMem(0x23c0 + i/32 + (scanline/32)*8) >> 6;
            }
            else {
                if((i/8) & 1)
                    palleteNumber = readFromMem(0x23c0 + i/32 + (scanline/32)*8) & 3;
                else
                    palleteNumber = (readFromMem(0x23c0 + i/32 + (scanline/32)*8) >> 2) & 3;
            }
            int patterTablePart = (((readFromMem(offset + address*(spriteSize*2) + scanline % spriteSize) & (1 << (7 - (i % 8)))) >> (7 - (i % 8)))) 
                                   + (((readFromMem(offset + address*(spriteSize*2) + scanline % spriteSize + spriteSize) & (1 << (7 - (i % 8)))) >> (7 - (i % 8))) << 1);
            //int patterTablePart = readFromMem(0x1000 + address*(spriteSize*2) + scanline % spriteSize)
            //std::cout << " i " << (int)i << " scanline " << scanline << " palnumn " << (int)palleteNumber;
            //std::cout << "first " << (int)readFromMem(0x1000 + address*(spriteSize*2) + scanline % spriteSize) << " second " << (int)readFromMem(0x1000 + address*(spriteSize*2) + scanline % spriteSize + spriteSize) << " " << (int)patterTablePart << " ";
            SDL_SetRenderDrawColor(renderer, palleteColors::pallete[imagePallete[patterTablePart][palleteNumber]].r,
                                             palleteColors::pallete[imagePallete[patterTablePart][palleteNumber]].g,
                                             palleteColors::pallete[imagePallete[patterTablePart][palleteNumber]].b, 255);
            SDL_RenderDrawPoint(renderer, i, scanline);
        }
        //for(int i=0; i<32; i++)
        //std::cout << (int)readFromMem(0x1000 + 0x10*(spriteSize*2) + scanline % spriteSize) << " ";
        //std::cout << std::endl;
        
    }
    else if (scanline == 241) {
        SDL_RenderPresent(renderer);
        *registers[2] |= 0x80;
        //*registers[0] |= 0x80;
        //std::cout << "NMI!!!" << std::endl;
    }
    else if(scanline == 261) {
        *registers[2] &= 0x7f;
        scanline = -1;
        //std::cout << "stop NMI!!!" << std::endl;
    }
    scanline++;
    cycles += 341;
}