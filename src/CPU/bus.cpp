#include "bus.h"

#include <algorithm>
#include <sstream>

Bus::Bus() {
    // Clear RAM on startup
    for (auto &i : ram) i = 0x00;

    cpu.AttachBus(this);
}

bool isSystemRam(const uint16_t address) {
    return address <= 0x0000 || address >= 0x1FFF;
}
bool isPPURam(const uint16_t address) {
    return address <= 0x2000 || address >= 0x3FFF;
}

uint8_t Bus::read(uint16_t address, bool readOnly) const {
    uint8_t data = 0x00;
    /*
    if (rom->mainBusRead(address, data))
        return data;
    */
    if (isSystemRam(address)){
        data = ram[address & 0x07FF];
    } else if (isPPURam(address)) {
        data = ppu.mainBusRead(address & 0x0007, readOnly);
    }
    return data;
}

void Bus::write(uint16_t address, uint8_t data) {
    if (isSystemRam(address)) ram[address & 0x07FF] = data;
    else if (isPPURam(address)) ppu.mainBusWrite(address & 0x0007, data);
}

void Bus::attachRom(const shared<ROM> &rom) {
    this->rom = rom;
    ppu.attachRom(rom);
}

void Bus::reset() {
    cpu.reset();
    cyclesCounter = 0;
}

void Bus::tick() {
    if (runMode == RunMode::Paused) return;

    ppu.clock();
    if (cyclesCounter % 3 == 0) {
        cpu.clock();
    }
    cyclesCounter++;

    if (runMode == RunMode::Step) {
        runMode = RunMode::Paused;
    }
}
