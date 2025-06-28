//
// NESkvik - Fredrik Ofstad
//

#include "mapper_0.h"

#define MIN_ADDR 0x8000
#define MAX_CPU_ADDR 0xFFFF
#define MAX_PPU_ADDR 0x1FFF
#define ROM_16K 0x3FFF
#define ROM_32K 0x7FFF

Mapper_0::Mapper_0(uint8_t programBanks, uint8_t charBanks):
Mapper(programBanks, charBanks) {}

Mapper_0::~Mapper_0() = default;

bool checkAddress(uint16_t address, uint16_t max) {
    return address >= MIN_ADDR && address <= max;
}


bool Mapper_0::cpuMapRead(uint16_t address, uint32_t &mapped_address) {
    if (checkAddress(address, MAX_CPU_ADDR)) {
        mapped_address = address & (programBanks > 1 ?  ROM_32K : ROM_16K);
        return true;
    }
    return false;
}

bool Mapper_0::cpuMapWrite(uint16_t address, uint32_t &mapped_address) {
    if (checkAddress(address, MAX_CPU_ADDR)) {
        mapped_address = address & (programBanks > 1 ?  ROM_32K : ROM_16K);
        return true;
    }
    return false;
}

bool Mapper_0::ppuMapRead(uint16_t address, uint32_t &mapped_address) {
    if (checkAddress(address, MAX_PPU_ADDR)) {
        mapped_address = address;
        return true;
    }
    return false;
}

bool Mapper_0::ppuMapWrite(uint16_t address, uint32_t &mapped_address) {
    return false;
}
