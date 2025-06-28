//
// NESkvik - Fredrik Ofstad
// Emulation of a olc2C02 Chip
//

#pragma once

#include "../types.h"


class ROM;

class PPU {

public:

    uint8_t mainBusRead(uint16_t address, bool readOnly = false) const;
    void mainBusWrite(uint16_t address, uint8_t data);

    uint8_t ppuBusRead(uint16_t address, bool readOnly = false);
    void ppuBusWrite(uint16_t address, uint8_t data);

    void attachRom(shared<ROM> rom);

    void clock();


private:
    //vram
    uint8_t nameTable[2][1024];
    uint8_t paletteTable[32];

    shared<ROM> rom;
};


/*
// Assume pixels is a uint32_t[256 * 240] array with RGBA values
gui.updateFrameBuffer(pixels, 256 * sizeof(uint32_t));
 */
