//
// NESkvikk - Fredrik Ofstad
//

#include "opcodes.h"

#include "address_modes.h"
#include "cpu.h"
#include "lookup.h"

namespace opcodes {

// Arithmetic
uint8_t ADC(CPU& cpu) {
    cpu.fetch();

    cpu.store = static_cast<uint16_t>(cpu.a) + static_cast<uint16_t>(cpu.fetched_data) +
        static_cast<uint16_t>(cpu.getFlag(flags::Carry));

    cpu.setFlag(flags::Carry, cpu.store > 255);
    cpu.setFlag(flags::Zero, (cpu.store & 0x00FF) == 0);

    cpu.setFlag(flags::Overflow, (~(static_cast<uint16_t>(cpu.a) ^ static_cast<uint16_t>(cpu.fetched_data)) &
        (static_cast<uint16_t>(cpu.a) ^ static_cast<uint16_t>(cpu.store))) & 0x0080);

    cpu.setFlag(flags::Negative, cpu.store & 0x80);
    cpu.a = cpu.store & 0x00FF;

    return 1;
}
uint8_t SBC(CPU& cpu) {
    cpu.fetch();

    uint16_t value = static_cast<uint16_t>(cpu.fetched_data) ^ 0x00FF;

    cpu.store = static_cast<uint16_t>(cpu.a) + value +
        static_cast<uint16_t>(cpu.getFlag(flags::Carry));

    cpu.setFlag(flags::Carry, cpu.store > 255);
    cpu.setFlag(flags::Zero, (cpu.store & 0x00FF) == 0);

    cpu.setFlag(flags::Overflow, (~(static_cast<uint16_t>(cpu.a) ^ static_cast<uint16_t>(cpu.fetched_data)) &
        (static_cast<uint16_t>(cpu.a) ^ static_cast<uint16_t>(cpu.store))) & 0x0080);

    cpu.setFlag(flags::Negative, cpu.store & 0x80);
    cpu.a = cpu.store & 0x00FF;

    return 1;
}

// Logic
uint8_t AND(CPU& cpu) {
    cpu.fetch();
    cpu.a = cpu.a & cpu.fetched_data;
    cpu.setFlag(flags::Zero, cpu.a == 0x00);
    cpu.setFlag(flags::Negative, cpu.a & 0x80);
    return 1;
}

uint8_t ORA(CPU& cpu) {
    cpu.fetch();
    cpu.a = cpu.a | cpu.fetched_data;
    cpu.setFlag(flags::Zero, cpu.a == 0x00);
    cpu.setFlag(flags::Negative, cpu.a & 0x80);
    return 1;
}
uint8_t EOR(CPU& cpu) {
    cpu.fetch();
    cpu.a = cpu.a ^ cpu.fetched_data;
    cpu.setFlag(flags::Zero, cpu.a == 0x00);
    cpu.setFlag(flags::Negative, cpu.a & 0x80);
    return 1;
}

uint8_t BIT(CPU& cpu) {
    cpu.fetch();
    cpu.store = cpu.a & cpu.fetched_data;
    cpu.setFlag(flags::Zero, (cpu.store & 0x00FF) == 0x00);
    cpu.setFlag(flags::Negative, cpu.fetched_data & (1 << 7));
    cpu.setFlag(flags::Overflow, cpu.fetched_data & (1 << 6));
    return 0;
}

// Shifts & Rotates
uint8_t ASL(CPU& cpu) {
    cpu.fetch();
    cpu.store = static_cast<uint16_t>(cpu.fetched_data) << 1;
    cpu.setFlag(flags::Carry, (cpu.store & 0xFF00) > 0);
    cpu.setFlag(flags::Zero, (cpu.store & 0x00FF) == 0x00);
    cpu.setFlag(flags::Negative, cpu.store & 0x80);
    if (lookup[cpu.current_opcode].addressmode != address_mode::IMP)
        cpu.a = cpu.store & 0x00FF;
    else
        cpu.write(cpu.address_abs, cpu.store & 0x00FF);
    return 0;
}

uint8_t LSR(CPU& cpu) {
    cpu.fetch();
    cpu.setFlag(flags::Carry, cpu.fetched_data & 0x0001);
    cpu.store = cpu.fetched_data >> 1;
    cpu.setFlag(flags::Zero, (cpu.store & 0x00FF) == 0x0000);
    cpu.setFlag(flags::Negative, cpu.store & 0x0080);
    if (isAddressMode(cpu, address_mode::IMP))
        cpu.a = cpu.store & 0x00FF;
    else
        cpu.write(cpu.address_abs, cpu.store & 0x00FF);
    return 0;
}

uint8_t ROL(CPU& cpu) {
    cpu.fetch();
    cpu.store = static_cast<uint16_t>(cpu.fetched_data) << 1 | cpu.getFlag(flags::Carry);
    cpu.setFlag(flags::Carry, cpu.fetched_data & 0xFF00);
    cpu.setFlag(flags::Zero, (cpu.store & 0x00FF) == 0x0000);
    cpu.setFlag(flags::Negative, cpu.store & 0x0080);
    if (isAddressMode(cpu, address_mode::IMP))
        cpu.a = cpu.store & 0x00FF;
    else
        cpu.write(cpu.address_abs, cpu.store & 0x00FF);
    return 0;
}
uint8_t ROR(CPU& cpu) {
    cpu.fetch();
    cpu.store = static_cast<uint16_t>(cpu.getFlag(flags::Carry)) << 7 | cpu.fetched_data >> 1;
    cpu.setFlag(flags::Carry, cpu.fetched_data & 0x01);
    cpu.setFlag(flags::Zero, (cpu.store & 0x00FF) == 0x00);
    cpu.setFlag(flags::Negative, cpu.store & 0x0080);
    if (isAddressMode(cpu, address_mode::IMP))
        cpu.a = cpu.store & 0x00FF;
    else
        cpu.write(cpu.address_abs, cpu.store & 0x00FF);
    return 0;
}

// Increments & Decrements
uint8_t INC(CPU& cpu) {
    cpu.fetch();
    cpu.store = cpu.fetched_data + 1;
    cpu.write(cpu.address_abs, cpu.store & 0x00FF);
    cpu.setFlag(flags::Zero, (cpu.store & 0x00FF) == 0x0000);
    cpu.setFlag(flags::Negative, cpu.store & 0x0080);
    return 0;
}
uint8_t INX(CPU& cpu) {
    cpu.x++;
    cpu.setFlag(flags::Zero, cpu.x == 0x00);
    cpu.setFlag(flags::Negative, cpu.x & 0x80);
    return 0;
}
uint8_t INY(CPU& cpu) {
    cpu.y++;
    cpu.setFlag(flags::Zero, cpu.x == 0x00);
    cpu.setFlag(flags::Negative, cpu.x & 0x80);
    return 0;
}
uint8_t DEC(CPU& cpu) {
    cpu.fetch();
    cpu.store = cpu.fetched_data - 1;
    cpu.write(cpu.address_abs, cpu.store & 0x00FF);
    cpu.setFlag(flags::Zero, (cpu.store & 0x00FF) == 0x0000);
    cpu.setFlag(flags::Negative, cpu.store & 0x0080);
    return 0;
}
uint8_t DEX(CPU& cpu) {
    cpu.x--;
    cpu.setFlag(flags::Zero, cpu.x == 0x00);
    cpu.setFlag(flags::Negative, cpu.x & 0x80);
    return 0;
}
uint8_t DEY(CPU& cpu) {
    cpu.y--;
    cpu.setFlag(flags::Zero, cpu.y == 0x00);
    cpu.setFlag(flags::Negative, cpu.y & 0x80);
    return 0;
}

// Jumps & Branches
uint8_t JMP(CPU& cpu) {
    cpu.pc = cpu.address_abs;
    return 0;
}
uint8_t JSR(CPU& cpu) {
    cpu.pc--;
    cpu.write(0x0100 + cpu.sp, (cpu.pc >> 8) & 0x00FF);
    cpu.sp--;
    cpu.write(0x0100 + cpu.sp, cpu.pc & 0x00FF);
    cpu.sp--;

    cpu.pc = cpu.address_abs;
    return 0;
}
uint8_t RTS(CPU& cpu) {
    cpu.sp++;
    cpu.pc = static_cast<uint16_t>(cpu.read(0x0100 + cpu.sp));
    cpu.sp++;
    cpu.pc |= static_cast<uint16_t>(cpu.read(0x0100 + cpu.sp)) << 8;

    cpu.pc++;
    return 0;
}
uint8_t RTI(CPU& cpu) {
    cpu.sp++;
    cpu.status = cpu.read(0x0100 + cpu.sp);
    cpu.status &= ~flags::Break;
    cpu.status &= ~flags::Unused;

    cpu.sp++;
    cpu.pc = static_cast<uint16_t>(cpu.read(0x0100 + cpu.sp));
    cpu.sp++;
    cpu.pc |= static_cast<uint16_t>(cpu.read(0x0100 + cpu.sp)) << 8;
    return 0;
}

uint8_t BCC(CPU& cpu) {
    cpu.branchIf(cpu.getFlag(flags::Carry) == 0);
    return 0;
}
uint8_t BCS(CPU& cpu) {
    cpu.branchIf(cpu.getFlag(flags::Carry) == 1);
    return 0;
}
uint8_t BEQ(CPU& cpu) {
    cpu.branchIf(cpu.getFlag(flags::Zero) == 1);
    return 0;
}
uint8_t BMI(CPU& cpu) {
    cpu.branchIf(cpu.getFlag(flags::Negative) == 1);
    return 0;
}
uint8_t BNE(CPU& cpu) {
    cpu.branchIf(cpu.getFlag(flags::Zero) == 0);
    return 0;
}
uint8_t BPL(CPU& cpu) {
    cpu.branchIf(cpu.getFlag(flags::Negative) == 0);
    return 0;
}
uint8_t BVC(CPU& cpu) {
    cpu.branchIf(cpu.getFlag(flags::Overflow) == 0);
    return 0;
}
uint8_t BVS(CPU& cpu) {
    cpu.branchIf(cpu.getFlag(flags::Overflow) == 1);
    return 0;
}

// Loads
uint8_t LDA(CPU& cpu) {
    return cpu.load(cpu.a);
}
uint8_t LDX(CPU& cpu) {
    return cpu.load(cpu.x);
}
uint8_t LDY(CPU& cpu) {
    return cpu.load(cpu.y);
}

// Stores
uint8_t STA(CPU& cpu) {
    cpu.write(cpu.address_abs, cpu.a);
    return 0;
}
uint8_t STX(CPU& cpu) {
    cpu.write(cpu.address_abs, cpu.x);
    return 0;
}
uint8_t STY(CPU& cpu) {
    cpu.write(cpu.address_abs, cpu.y);
    return 0;
}

// Flag manipulation
uint8_t CLC(CPU& cpu) {
    cpu.setFlag(flags::Carry, false);
    return 0;
}

uint8_t CLD(CPU& cpu) {
    cpu.setFlag(flags::Decimal, false);
    return 0;
}

uint8_t CLI(CPU& cpu) {
    cpu.setFlag(flags::Interrupts, false);
    return 0;
}
uint8_t CLV(CPU& cpu) {
    cpu.setFlag(flags::Overflow, false);
    return 0;
}
// Set flags
uint8_t SEC(CPU& cpu) {
    cpu.setFlag(flags::Carry, true);
    return 0;
}
uint8_t SED(CPU& cpu) {
    cpu.setFlag(flags::Decimal, true);
    return 0;
}
uint8_t SEI(CPU& cpu) {
    cpu.setFlag(flags::Interrupts, true);
    return 0;
}

// Stack operations
uint8_t PHA(CPU& cpu) {
    // 0x0100 stack location
    cpu.write(0x0100 + cpu.sp, cpu.a);
    cpu.sp--;
    return 0;
}
uint8_t PHP(CPU& cpu) {
    cpu.write(0x0100 + cpu.sp, cpu.status | flags::Break | flags::Unused);
    cpu.setFlag(flags::Break, false);
    cpu.setFlag(flags::Unused, false);
    cpu.sp--;
    return 0;
}
uint8_t PLA(CPU& cpu) {
    cpu.sp++;
    cpu.a = cpu.read(0x0100 + cpu.sp);
    cpu.setFlag(flags::Zero, cpu.a == 0x00);
    cpu.setFlag(flags::Negative, cpu.a & 0x80);
    return 0;
}
uint8_t PLP(CPU& cpu) {
    cpu.sp++;
    cpu.status = cpu.read(0x0100 + cpu.sp);
    cpu.setFlag(flags::Unused, true);
    return 0;
}

// Register transfers
uint8_t TAX(CPU& cpu) {
    cpu.x = cpu.a;
    cpu.setFlag(flags::Zero, cpu.x == 0x00);
    cpu.setFlag(flags::Negative, cpu.x & 0x80);
    return 0;
}
uint8_t TAY(CPU& cpu) {
    cpu.y = cpu.a;
    cpu.setFlag(flags::Zero, cpu.y == 0x00);
    cpu.setFlag(flags::Negative, cpu.y & 0x80);
    return 0;
}
uint8_t TSX(CPU& cpu) {
    cpu.x = cpu.sp;
    cpu.setFlag(flags::Zero, cpu.x == 0x00);
    cpu.setFlag(flags::Negative, cpu.x & 0x80);
    return 0;
}
uint8_t TXA(CPU& cpu) {
    cpu.a = cpu.x;
    cpu.setFlag(flags::Zero, cpu.a == 0x00);
    cpu.setFlag(flags::Negative, cpu.a & 0x80);
    return 0;
}
uint8_t TXS(CPU& cpu) {
    cpu.sp = cpu.x;
    return 0;
}
uint8_t TYA(CPU& cpu) {
    cpu.a = cpu.y;
    cpu.setFlag(flags::Zero, cpu.a == 0x00);
    cpu.setFlag(flags::Negative, cpu.a & 0x80);
    return 0;
}

// Comparisons
    uint8_t CMP(CPU& cpu) {
    cpu.compare(cpu.a);
    return 1; // Additional cycle cost
}

    uint8_t CPX(CPU& cpu) {
    cpu.compare(cpu.x);
    return 0;
}

    uint8_t CPY(CPU& cpu) {
    cpu.compare(cpu.y);
    return 0;
}

// System
uint8_t BRK(CPU& cpu) {
    cpu.pc++;

    cpu.setFlag(flags::Interrupts, true);
    cpu.write(0x0100 + cpu.sp, (cpu.pc >> 8) & 0x00FF);
    cpu.sp--;
    cpu.write(0x0100 + cpu.sp, cpu.pc & 0x00FF);
    cpu.sp--;

    cpu.setFlag(flags::Break, true);
    cpu.write(0x0100 + cpu.sp, cpu.status);
    cpu.sp--;
    cpu.setFlag(flags::Break, false);

    cpu.pc = (uint16_t)(cpu.read(0xFFFE)) | (
        (uint16_t)(cpu.read(0xFFFF)) << 8);
    return 0;

}
uint8_t NOP(CPU& cpu) {
    // some cases need an extra cycle
    switch (cpu.current_opcode) {
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            return 1;
        default: return 0;
    }
}

//Catch all
uint8_t XXX(CPU& cpu) {
    return 0;
}
}
