#include "cpu.h"

void Cpu::fetchOpcode() {
    uint8_t opcode = readFromMem(PC_reg);
    std::cout << "Current opcode: " << (int)opcode << std::endl;
    switch(opcode) {
        case 0x00:
            BRK();
            break;
        case 0x1:
            ORA(IndexedIndirect);
            break;
        case 0x5:
            ORA(ZeroPage);
            break;
        case 0x6:
            ASL(ZeroPage);
            break;
        case 0x8:
            PHP();
            break;
        case 0x9:
            ORA(Immediate);
            break;
        case 0xa:
            ASL(Accumulator);
            break;
        case 0xd:
            ORA(Absolute);
            break;
        case 0xe:
            ASL(Absolute);
            break;
        case 0x10:
            BPL();
            break;
        case 0x11:
            ORA(IndirectIndexed);
            break;
        case 0x15:
            ORA(ZeroPageIndexed);
            break;
        case 0x16:
            ASL(ZeroPageIndexed);
            break;
        case 0x18:
            CLC();
            break;
        case 0x19:
            ORA(AbsoluteIndexedY);
            break;
        case 0x1d:
            ORA(AbsoluteIndexedX);
            break;
        case 0x1e:
            ASL(AbsoluteIndexedX);
            break;
        case 0x20:
            JSR();
            break;
        case 0x21:
            AND(IndexedIndirect);
            break;
        case 0x24:
            BIT(ZeroPage);
            break;
        case 0x25:
            AND(ZeroPage);
            break;
        case 0x26:
            ROL(ZeroPage);
            break;
        case 0x28:
            PLP();
            break;
        case 0x29:
            AND(Immediate);
            break;
        case 0x2a:
            ROL(Accumulator);
            break;
        case 0x2c:
            BIT(Absolute);
            break;
        case 0x2d:
            AND(Absolute);
            break;
        case 0x2e:
            ROL(Absolute);
            break;
        case 0x30:
            BMI();
            break;
        case 0x31:
            AND(IndirectIndexed);
            break;
        case 0x35:
            AND(ZeroPageIndexed);
            break;
        case 0x36:
            ROL(ZeroPageIndexed);
            break;
        case 0x38:
            SEC();
            break;
        case 0x39:
            AND(AbsoluteIndexedY);
            break;
        case 0x40:
            RTI();
            break;
        case 0x41:
            EOR(IndexedIndirect);
            break;
        case 0x45:
            EOR(ZeroPage);
            break;
        case 0x46:
            LSR(ZeroPage);
            break;
        case 0x48:
            PHA();
            break;
        case 0x49:
            EOR(Immediate);
            break;
        case 0x4a:
            LSR(Accumulator);
            break;
        case 0x4c:
            JMP(Absolute);
            break;
        case 0x4d:
            EOR(Absolute);
            break;
        case 0x4e:
            LSR(Absolute);
            break;
        case 0x50:
            BVC();
            break;
        case 0x51:
            EOR(IndirectIndexed);
            break;
        case 0x55:
            EOR(ZeroPageIndexed);
            break;
        case 0x56:
            LSR(ZeroPageIndexed);
            break;
        case 0x58:
            CLI();
            break;
        case 0x59:
            EOR(AbsoluteIndexedY);
            break;
        case 0x5d:
            EOR(AbsoluteIndexedX);
            break;
        case 0x5e:
            LSR(AbsoluteIndexedX);
            break;
        case 0x60:
            RTS();
            break;
        case 0x61:
            ADC(IndexedIndirect);
            break;
        default:
            std::cout << "Instruction undefined" << std::endl;
    }
}

//System instructions

void Cpu::BRK() {
    setFlag(1, Interupt);
    push(PC_reg+2);
    push(SP_reg);
    PC_reg++;
    cycles += 7;
}
