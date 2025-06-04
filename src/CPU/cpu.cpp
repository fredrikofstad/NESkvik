#include "cpu.h"

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
}
