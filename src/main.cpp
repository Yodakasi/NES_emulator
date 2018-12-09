#include <iostream>
#include <cstdint>
#include "nes.h"

int main() {
    Nes nes;
    int i = 0;
    while(nes.isRunning()) {
        i++;
        nes.run();
        if(i>100) break;
    }
    nes.dump();
    /*
    cpu.dumpReg();
    cpu.dumpStack();
    //cpu.fetchOpcode();
    cpu.dumpMem();
    //cpu.dumpReg();
    */
}