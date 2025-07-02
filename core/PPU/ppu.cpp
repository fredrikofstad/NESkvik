//
// NESkvikk - Fredrik Ofstad
//

#include "ppu.h"


PPU::PPU() {

}



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
        case 0x000:
            control.reg = data;
            tram_address.nametable_x = control.nametable_x;
            tram_address.nametable_y = control.nametable_y;
            break;
        case 0x001:
            control.reg = data;
            break;
        case 0x002: break;
        case 0x003: break;
        case 0x004: break;
        case 0x005:
            if (address_latch == 0) {
                x_offset = data & 0x07;
                tram_address.coarse_x = data >> 3;
                address_latch = 1;
            } else {
                tram_address.y_offset = data & 0x07;
                tram_address.coarse_y = data >> 3;
                address_latch = 0;
            }
            break;
        case 0x006:
            if (address_latch == 0) {
                tram_address.reg = static_cast<uint16_t>((data & 0x3F) << 8) | (tram_address.reg & 0x00FF);
                address_latch = 1;
            } else {
                tram_address.reg = (tram_address.reg & 0xFF00) | data;
                vram_address = tram_address;
                address_latch = 0;
            }
            break;
        case 0x007:
            ppuBusWrite(vram_address.reg, data);
            vram_address.reg += (control.increment_mode ? 32 : 1);
            break;
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

void PPU::reset() {
    x_offset = 0x00;
    address_latch = 0x00;
    ppu_data_buffer = 0x00;
    scanline = 0;
    cycle = 0;
    bg_next_tile_id = 0x00;
    bg_next_tile_attrib = 0x00;
    bg_next_tile_lsb = 0x00;
    bg_next_tile_msb = 0x00;
    bg_shifter_pattern_lo = 0x0000;
    bg_shifter_pattern_hi = 0x0000;
    bg_shifter_attrib_lo = 0x0000;
    bg_shifter_attrib_hi = 0x0000;
    status.reg = 0x00;
    mask.reg = 0x00;
    control.reg = 0x00;
    vram_address.reg = 0x0000;
    tram_address.reg = 0x0000;
}

