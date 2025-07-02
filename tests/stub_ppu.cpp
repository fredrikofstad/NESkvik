//
// NESkvik - Fredrik Ofstad
//

#include "../core/PPU/ppu.h"

PPU::PPU() {
    // empty constructor stub
}

uint8_t PPU::mainBusRead(uint16_t /*address*/, bool /*readOnly*/) const {
    return 0;
}

void PPU::mainBusWrite(uint16_t /*address*/, uint8_t /*data*/) {
    // stub does nothing
}

uint8_t PPU::ppuBusRead(uint16_t /*address*/, bool /*readOnly*/) {
    return 0;
}

void PPU::ppuBusWrite(uint16_t /*address*/, uint8_t /*data*/) {
    // stub does nothing
}

void PPU::attachRom(shared<ROM> /*rom*/) {
    // stub does nothing
}

void PPU::clock() {
    // stub does nothing
}

void PPU::reset() {
    // stub does nothing
}
