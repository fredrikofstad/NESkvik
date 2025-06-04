#include "bus.h"

bus::bus() {
    // Clear RAM on startup
    for (auto &i : ram) i = 0x00;

    cpu.AttachBus(this);
}

bool isOutsideRange(const uint16_t address) {
    return address <= 0x0000 || address >= 0x0FFF;
}

void bus::write(uint16_t address, uint8_t data) {
    if (isOutsideRange(address)) return;
    ram[address] = data;
}

uint8_t bus::read(uint16_t address, bool readOnly) const {
    if (isOutsideRange(address)) return 0x00;
    return ram[address];
}




