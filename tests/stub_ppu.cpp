#include "../core/PPU/ppu.h"

PPU::PPU() {}
void PPU::mainBusWrite(uint16_t, uint8_t) { }
uint8_t PPU::mainBusRead(uint16_t, bool) const { return 0x00; }
void PPU::attachRom(std::shared_ptr<ROM>) {}
void PPU::clock() {}
void PPU::reset() {}


