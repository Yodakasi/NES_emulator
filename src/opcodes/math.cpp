#include "../cpu.h"

void Cpu::ADC(int addressingMode) {
    uint8_t value;
    switch(addressingMode) {
        case IndexedIndirect:
            value = readFromMem(indexedIndirect(readFromMem(PC_reg+1)));
            cycles += 6;
            PC_reg += 2;
            break;
    }
    uint16_t tmp = value + A_reg + getFlag(Carry);
    std::cout << "wynik " << (int)tmp << std::endl;
    uint8_t result = tmp;
    std::cout << "wynik2 " << (int)((tmp & 256) >> 8) << std::endl;
    setFlag((tmp & 256) >> 8, Carry);
    setFlag((tmp & 256) >> 8, Overflow);
    if(result > 0x7f)
        setFlag(1, Negative);
    else
        setFlag(0, Negative);
    if(result == 0)
        setFlag(1, Zero);
    else
        setFlag(0, Zero);
    A_reg = result;

}