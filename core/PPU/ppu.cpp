//
// NESkvikk - Fredrik Ofstad
//

#include "ppu.h"

uint8_t PPU::mainBusRead(uint16_t address, bool readOnly) const {
    switch (address) {
        case 0x000: break;
        case 0x001: break;
        case 0x002: break;
        case 0x003: break;
        case 0x004: break;
        case 0x005: break;
        case 0x006: break;
        case 0x007: break;
        default: break;
    }
    return 0x00;
}

void PPU::mainBusWrite(uint16_t address, uint8_t data) {
    switch (address) {
        case 0x000: break;
        case 0x001: break;
        case 0x002: break;
        case 0x003: break;
        case 0x004: break;
        case 0x005: break;
        case 0x006: break;
        case 0x007: break;
        default: break;
    }
}

uint8_t PPU::ppuBusRead(uint16_t address, bool readOnly) {
    uint8_t data = 0x00;
    address &= 0x3FFF;
    return data;
}

void PPU::ppuBusWrite(uint16_t address, uint8_t data) {
    address &= 0x3FFF;
}

void PPU::attachRom(shared<ROM> rom) {
    this->rom = rom;
}

void PPU::clock() {
}
