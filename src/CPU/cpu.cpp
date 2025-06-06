#include "cpu.h"
#include "lookup.h"

CPU::CPU() {
}

uint8_t CPU::read(uint8_t address) const {
    return bus->read(address);
}

void CPU::write(uint8_t address, uint8_t data) {
    return bus->write(address, data);
}

uint8_t CPU::getFlag(flags flag) {
}

void CPU::setFlag(flags flag, bool value) {
    if (value) {
        status |= flag;
    } else {
        status &= ~flag;
    }
}

void CPU::clock() {
    if (cycles == 0) {
        current_opcode = read(pc);
        pc++;

        cycles = std::get<3>(lookup[current_opcode]);    // get cycles needed
        uint8_t extra_cycles_addr = std::get<2>(lookup[current_opcode])( *this ); // call address mode function
        uint8_t extra_cycles_op = std::get<1>(lookup[current_opcode])( *this );   // call opcode function
        cycles += (extra_cycles_addr & extra_cycles_op);
    }
    cycles--;
}

