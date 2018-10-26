#include <iostream>
#include <cstdint>
#include "cpu.h"

int main() {
    Cpu cpu;
    std::cout << cpu.getCpuState() << std::endl;
}