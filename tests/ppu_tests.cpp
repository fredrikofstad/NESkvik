#include "../core/PPU/ppu.h"
#include <cstdio>
#include "../core/ROM/rom.h"

int main() {


    PPU ppu;

    std::string romPath = R"(C:\Dev\C\NESkvik\tests\roms\nestest.nes)"; // Adjust this to your ROM path
    auto rom = std::make_shared<ROM>(romPath);

    ppu.attachRom(rom);

    // Initialize pattern tables
    for (int i = 0; i < 4096; i++) {
        ppu.loadPatternTable(0, i, 0x00);  // tile 0 low plane
        ppu.loadPatternTable(1, i, 0xFF);  // tile 1 low plane (example)
    }

    // Initialize nametables
    for (int i = 0; i < 1024; i++) {
        ppu.loadNameTable(0, i, 0x00);  // tile 0 everywhere
    }

    // Initialize palette
    for (int i = 0; i < 32; i++) {
        ppu.loadPalette(i, i & 0x3F);  // simple ramp palette for test
    }

    // Setup control and mask registers
    ppu.setControlReg(0x00);  // example: pattern table 0 for background
    ppu.setMaskReg(0x08);     // enable background rendering

    // Run a few scanlines and cycles to "clock" the PPU
    for (int scanline = -1; scanline < 240; scanline++) {
        ppu.setScanline(scanline);
        for (int cycle = 0; cycle < 341; cycle++) {
            ppu.setCycle(cycle);
            ppu.clock();
        }
    }

    // Print top-left 16x10 pixels of the framebuffer as hex values
    printf("Framebuffer snapshot (top-left 16x10 pixels):\n");
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 16; x++) {
            uint8_t pixel = ppu.getFramebufferPixel(x, y);
            printf("%02X ", pixel);
        }
        printf("\n");
    }

    return 0;
}
