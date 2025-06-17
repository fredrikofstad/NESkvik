#pragma once
#include <array>
#include <cstdint>

#include "cpu.h"
#include "../types.h"
#include "../PPU/ppu.h"
#include "../ROM/rom.h"

#define RAM_SIZE (2048)

class Bus {
public:
    Bus();
    CPU cpu;
    PPU ppu;
    shared<ROM> rom;
    std::array<uint8_t, RAM_SIZE> ram;

    void write(uint16_t address, uint8_t data);
    uint8_t read(uint16_t address, bool readOnly = false) const;

    void attachRom(const shared<ROM> &rom);
    void reset();
    void tick();

private:
    uint32_t cyclesCounter;
};
