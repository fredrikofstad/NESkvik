//
// NESkvik - Fredrik Ofstad
// Emulation of a olc2C02 Chip
//

#pragma once

#include "../types.h"
#include "ppu_registers.h"


class ROM;

class PPU {

public:

    PPU();

    uint8_t mainBusRead(uint16_t address, bool readOnly = false) const;
    void mainBusWrite(uint16_t address, uint8_t data);

    uint8_t ppuBusRead(uint16_t address, bool readOnly = false);
    void ppuBusWrite(uint16_t address, uint8_t data);


    void attachRom(shared<ROM> rom);
    void clock();
    void reset();

    bool nmi = false;

    bool frameComplete = false;



private:
    //vram
    uint8_t nameTable[2][1024];
    uint8_t paletteTable[32];
    uint8_t patternTable[2][4096];

    shared<ROM> rom;

    Status status;
    Mask mask;
    PPUCTRL control;

    LoopyRegister vram_address;
    LoopyRegister tram_address;

    uint8_t x_offset;

    uint8_t address_latch = 0x00;
    uint8_t ppu_data_buffer = 0x00;

    int16_t scanline = 0;
    int16_t cycle = 0;

    uint8_t bg_next_tile_id     = 0x00;
    uint8_t bg_next_tile_attrib = 0x00;
    uint8_t bg_next_tile_lsb    = 0x00;
    uint8_t bg_next_tile_msb    = 0x00;
    uint16_t bg_shifter_pattern_lo = 0x0000;
    uint16_t bg_shifter_pattern_hi = 0x0000;
    uint16_t bg_shifter_attrib_lo  = 0x0000;
    uint16_t bg_shifter_attrib_hi  = 0x0000;

};




/*
// Assume pixels is a uint32_t[256 * 240] array with RGBA values
gui.updateFrameBuffer(pixels, 256 * sizeof(uint32_t));
 */
