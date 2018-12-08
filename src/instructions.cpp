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
            branch(Negative, 0);
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
            setFlagOpcode(0, Carry);
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
            branch(Negative, 1);
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
            setFlagOpcode(1, Carry);
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
            branch(Overflow, 1);
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
            setFlagOpcode(0, Interupt);
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
        case 0x65:
            ADC(ZeroPage);
            break;
        case 0x66:
            ROR(ZeroPage);
            break;
        case 0x68:
            PLA();
            break;
        case 0x69:
            ADC(Immediate);
            break;
        case 0x6a:
            ROR(Accumulator);
            break;
        case 0x6c:
            JMP(Indirect);
            break;
        case 0x6d:
            ADC(Absolute);
            break;
        case 0x6e:
            ROR(Absolute);
            break;
        case 0x70:
            branch(Overflow, 1);
            break;
        case 0x71:
            ADC(IndirectIndexed);
            break;
        case 0x75:
            ADC(ZeroPageIndexed);
            break;
        case 0x76:
            ROR(ZeroPageIndexed);
            break;
        case 0x78:
            setFlagOpcode(1, Interupt);
            break;
        case 0x79:
            ADC(AbsoluteIndexedY);
            break;
        case 0x7d:
            ADC(AbsoluteIndexedX);
            break;
        case 0x7e:
            ROR(AbsoluteIndexedX);
            break;
        case 0x81:
            store(IndexedIndirect, A_reg);
            break;
        case 0x84:
            store(ZeroPage, Y_reg);
            break;
        case 0x85:
            store(ZeroPage, A_reg);
            break;
        case 0x86:
            store(ZeroPage, X_reg);
            break;
        case 0x90:
            branch(Carry, 0);
            break;
        case 0x91:
            store(IndirectIndexed, A_reg);
            break;
        case 0x94:
            store(ZeroPageIndexed, Y_reg, X_reg);
            break;
        case 0x95:
            store(ZeroPageIndexed, A_reg, X_reg);
            break;
        case 0x96:
            store(ZeroPageIndexed, X_reg, Y_reg);
            break;
        case 0x98:
            transfer(Y_reg, A);
            break;
        case 0x99:
            store(AbsoluteIndexedY, A_reg);
            break;
        case 0x9a:
            transfer(X_reg, SP);
            break;
        case 0x9d:
            store(AbsoluteIndexedX, A_reg);
            break;
        case 0xa0:
            load(Immediate, Y);
            break;
        case 0xa1:
            load(IndexedIndirect, A);
            break;
        case 0xa2:
            load(Immediate, X);
            break;
        case 0xa4:
            load(ZeroPage, Y);
            break;
        case 0xa5:
            load(ZeroPage, A);
            break;
        case 0xa6:
            load(ZeroPage, X);
            break;
        case 0xa8:
            transfer(A_reg, Y);
            break;
        case 0xa9:
            load(Immediate, A);
            break;
        case 0xaa:
            transfer(A_reg, X);
            break;
        case 0xac:
            load(Absolute, Y);
            break;
        case 0xad:
            load(Absolute, A);
            break;
        case 0xae:
            load(Absolute, X);
            break;
        case 0xb0:
            branch(Carry, 1);
            break;
        case 0xb1:
            load(IndirectIndexed, A);
            break;
        case 0xb4:
            load(ZeroPageIndexed, Y);
            break;
        case 0xb5:
            load(ZeroPageIndexed, A);
            break;
        case 0xb6:
            load(ZeroPageIndexed, X);
            break;
        case 0xb8:
            setFlagOpcode(0, Overflow);
            break;
        case 0xb9:
            load(AbsoluteIndexedY, A);
            break;
        case 0xba:
            transfer(SP_reg, X);
            break;
        case 0xbc:
            load(AbsoluteIndexedX, Y);
            break;
        case 0xbd:
            load(AbsoluteIndexedX, A);
            break;
        case 0xbe:
            load(AbsoluteIndexedY, X);
            break;
        case 0xc0:
            compare(Immediate, Y_reg);
            break;
        case 0xc1:
            compare(IndexedIndirect, A_reg);
            break;
        case 0xc4:
            compare(ZeroPage, Y_reg);
            break;
        case 0xc5:
            compare(ZeroPage, A_reg);
            break;
        case 0xc6:
            DECINC(ZeroPage, false);
            break;
        case 0xc8:
            INY();
            break;
        case 0xc9:
            compare(Immediate, A_reg);
            break;
        case 0xca:
            DEX();
            break;
        case 0xcc:
            compare(Absolute, Y_reg);
            break;
        case 0xcd:
            compare(Absolute, A_reg);
            break;
        case 0xce:
            DECINC(Absolute, false);
            break;
        case 0xd0:
            branch(Zero, 0);
            break;
        case 0xd1:
            compare(IndirectIndexed, A_reg);
            break;
        case 0xd5:
            compare(ZeroPageIndexed, A_reg);
            break;
        case 0xd6:
            DECINC(ZeroPageIndexed, false);
            break;
        case 0xd8:
            setFlagOpcode(0, Decimal);
            break;
        case 0xd9:
            compare(AbsoluteIndexedY, A_reg);
            break;
        case 0xdd:
            compare(AbsoluteIndexedX, A_reg);
            break;
        case 0xde:
            DECINC(AbsoluteIndexedX, false);
            break;
        case 0xe0:
            compare(Immediate, X_reg);
            break;
        case 0xe1:
            SBC(IndexedIndirect);
            break;
        case 0xe4:
            compare(ZeroPage, X_reg);
            break;
        case 0xe5:
            SBC(ZeroPage);
            break;
        case 0xe6:
            DECINC(ZeroPage, true);
            break;
        case 0xe8:
            INX();
            break;
        case 0xe9:
            SBC(Immediate);
            break;
        case 0xea:
            NOP();
            break;
        case 0xec:
            compare(Absolute, X_reg);
            break;
        case 0xed:
            SBC(Absolute);
            break;
        case 0xee:
            DECINC(Absolute, true);
            break;
        case 0xf0:
            branch(Zero, 1);
            break;
        case 0xf1:
            SBC(IndirectIndexed);
            break;
        case 0xf5:
            SBC(ZeroPageIndexed);
            break;
        case 0xf6:
            DECINC(ZeroPageIndexed, true);
            break;
        case 0xf8:
            setFlagOpcode(1, Decimal);
            break;
        case 0xf9:
            SBC(AbsoluteIndexedY);
            break;
        case 0xfd:
            SBC(AbsoluteIndexedX);
            break;
        case 0xfe:
            DECINC(AbsoluteIndexedX, true);
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

void Cpu::NOP() {
    PC_reg++;
    cycles += 2;
}
