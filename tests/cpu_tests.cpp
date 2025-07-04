#include "../core/PPU/ppu.h"
#include "../core/ROM/rom.h"
#include <cstdio>
#include <memory>

int main() {
    PPU ppu;

    // Load ROM (adjust path as needed)
    std::string romPath = R"(C:\Dev\C\NESkvik\tests\roms\nestest.nes)";
    auto rom = std::make_shared<ROM>(romPath);
    ppu.attachRom(rom);

    ppu.reset();

    // Load simple pattern table data for visible tiles
    // We'll create two tiles: tile 0 is empty, tile 1 is a checkerboard pattern
    // Each tile is 16 bytes: 8 bytes for bitplane 0 and 8 bytes for bitplane 1

    // Tile 0 (empty tile)
    for (int i = 0; i < 16; i++) {
        ppu.loadPatternTable(0, i, 0x00);
    }

    // Tile 1 (checkerboard pattern)
    uint8_t checkerboard_plane0[8] = { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 }; // 10101010 / 01010101
    uint8_t checkerboard_plane1[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // all zeros
    for (int i = 0; i < 8; i++) {
        ppu.loadPatternTable(0, 16 + i, checkerboard_plane0[i]);
        ppu.loadPatternTable(1, 16 + i, checkerboard_plane1[i]);
    }

    // Fill nametable with tile 1 to see checkerboards on the screen
    for (int i = 0; i < 1024; i++) {
        ppu.loadNameTable(0, i, 1);  // tile 1 index
    }

    // Setup a simple palette (index 1 = color 0x3F, index 0 = 0x00)
    for (int i = 0; i < 32; i++) {
        ppu.loadPalette(i, i & 0x3F);
    }

    // Setup control and mask to enable background rendering from pattern table 0
    ppu.setControlReg(0x00);  // Background pattern table 0
    ppu.setMaskReg(0x08);     // Enable background rendering

    // Run one full frame: 341 cycles * 261 scanlines
    const int totalCycles = 341 * 261;
    for (int i = 0; i < totalCycles; ++i) {
        ppu.clock();
    }

    // Print top-left 16x10 pixels of the framebuffer as palette indices (hex)
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
