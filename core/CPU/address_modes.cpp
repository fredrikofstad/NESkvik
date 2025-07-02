//
// NESkvik - Fredrik Ofstad
//

#include "address_modes.h"

#include <cstdio>

#include "cpu.h"

uint8_t address_mode::IMP(CPU &cpu) {
    cpu.fetched_data = cpu.a;
    return 0;
}

uint8_t address_mode::IMM(CPU &cpu) {
    cpu.address_abs = cpu.pc++;
    return 0;
}

uint8_t address_mode::ZP0(CPU &cpu) {
    cpu.address_abs = cpu.read(cpu.pc);
    cpu.pc++;
    cpu.address_abs &= 0x00FF;
    return 0;
}

uint8_t address_mode::ZPX(CPU &cpu) {
    cpu.address_abs = cpu.read(cpu.pc) + cpu.x;
    cpu.pc++;
    cpu.address_abs &= 0x00FF;
    return 0;
}

uint8_t address_mode::ZPY(CPU &cpu) {
    cpu.address_abs = cpu.read(cpu.pc) + cpu.y;
    cpu.pc++;
    cpu.address_abs &= 0x00FF;
    return 0;
}

uint8_t address_mode::ABS(CPU &cpu) {
    printf("address_mode::ABS invoked at PC=%04X\n", cpu.pc);
    uint16_t low = cpu.read(cpu.pc);
    cpu.pc++;
    uint16_t high = cpu.read(cpu.pc);
    cpu.pc++;
    cpu.address_abs = (high << 8) | low;
    return 0;
}

uint8_t address_mode::ABX(CPU &cpu) {
    uint16_t low = cpu.read(cpu.pc);
    cpu.pc++;
    uint16_t high = cpu.read(cpu.pc);
    cpu.pc++;

    cpu.address_abs = (high << 8) | low;
    cpu.address_abs += cpu.x;
    if ((cpu.address_abs & 0xFF00) != (high << 8))
        return 1;
    return 0;
}

uint8_t address_mode::ABY(CPU &cpu) {
    uint16_t low = cpu.read(cpu.pc);
    cpu.pc++;
    uint16_t high = cpu.read(cpu.pc);
    cpu.pc++;

    cpu.address_abs = (high << 8) | low;
    cpu.address_abs += cpu.y;
    if ((cpu.address_abs & 0xFF00) != (high << 8))
        return 1;
    return 0;
}

// pointers - indirect addressing

uint8_t address_mode::IND(CPU &cpu) {
    uint16_t low_pointer = cpu.read(cpu.pc);
    cpu.pc++;
    uint16_t high_pointer = cpu.read(cpu.pc);
    cpu.pc++;

    uint16_t pointer = (high_pointer << 8) | low_pointer;

    // hardware bug - page boundary bug
    if (low_pointer == 0x00FF) {
        cpu.address_abs = (cpu.read(pointer & 0xFF00) << 8) | cpu.read(pointer + 0);
    } else {
        cpu.address_abs = cpu.read((pointer + 1) << 8 | cpu.read(pointer + 0));
    }
    return 0;
}

uint8_t address_mode::IZX(CPU &cpu) {
    uint16_t addr = cpu.read(cpu.pc);
    cpu.pc++;
    uint16_t low = cpu.read(static_cast<uint16_t>(addr + static_cast<uint16_t>(cpu.x)) & 0x00FF);
    uint16_t high = cpu.read(static_cast<uint16_t>(addr + static_cast<uint16_t>(cpu.x) + 1) & 0x00FF);

    cpu.address_abs = (high << 8) | low;
    return 0;
}

uint8_t address_mode::IZY(CPU &cpu) {
    uint16_t addr = cpu.read(cpu.pc);
    cpu.pc++;
    uint16_t low = cpu.read(addr & 0x00FF);
    uint16_t high = cpu.read((addr + 1) & 0x00FF);

    cpu.address_abs = (high << 8) | low;
    cpu.address_abs += cpu.y;
    if ((cpu.address_abs & 0xFF00) != (high << 8))
        return 1;
    return 0;
}

uint8_t address_mode::REL(CPU &cpu) {
    cpu.address_rel = cpu.read(cpu.pc);
    cpu.pc++;
    if (cpu.address_rel & 0x80)
        cpu.address_rel |= 0xFF00;
    return 0;
}
