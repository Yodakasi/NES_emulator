#include <iostream>
#include <cstdint>
#include "cpu.h"

int main() {
    Cpu cpu;
    if(cpu.getCpuState()) {
        cpu.fetchOpcode();
        cpu.dumpReg();
        cpu.fetchOpcode();
        cpu.dumpMem();
        cpu.dumpReg();
    }
}