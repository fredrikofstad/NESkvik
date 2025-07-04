//
// NESkvikk - Fredrik Ofstad
//

#include "ppu.h"
#include "../ROM/rom.h"


PPU::PPU() {

}



uint8_t PPU::mainBusRead(uint16_t address, bool readOnly) const {
    uint8_t data = 0x00;

    // We need to cast away const for side effects, as this is a const method
    auto self = const_cast<PPU*>(this);

    if (readOnly) {
        // Read-only mode: examine register state without side effects
        switch (address) {
            case 0x0000:  // Control
                data = self->control.reg;
                break;
            case 0x0001:  // Mask
                data = self->mask.reg;
                break;
            case 0x0002:  // Status
                data = self->status.reg;
                break;
            case 0x0003:  // OAM Address
                // Typically unused or not readable; leave as 0
                break;
            case 0x0004:  // OAM Data
                break;
            case 0x0005:  // Scroll
                break;
            case 0x0006:  // PPU Address
                break;
            case 0x0007:  // PPU Data
                break;
        }
    } else {
        // Live read mode: registers may have side effects
        switch (address) {
            case 0x0000:  // Control - not readable
                break;

            case 0x0001:  // Mask - not readable
                break;

            case 0x0002:  // Status
                // Top 3 bits contain status, bottom 5 bits contain last bus data noise
                data = (self->status.reg & 0xE0) | (self->ppu_data_buffer & 0x1F);
                // Clear vertical blank flag
                self->status.vertical_blank = 0;
                // Reset address latch for Loopy registers
                self->address_latch = 0;
                break;

            case 0x0003:  // OAM Address - not readable
                break;

            case 0x0004:  // OAM Data - not implemented yet, leave 0
                break;

            case 0x0005:  // Scroll - not readable
                break;

            case 0x0006:  // PPU Address - not readable
                break;

            case 0x0007:  // PPU Data
                // Return buffered data first (delayed read)
                data = self->ppu_data_buffer;
                // Update buffer with current VRAM data
                self->ppu_data_buffer = self->ppuBusRead(self->vram_address.reg);
                // If reading palette memory, data not delayed
                if (self->vram_address.reg >= 0x3F00) {
                    data = self->ppu_data_buffer;
                }
                // Increment VRAM address depending on increment mode
                self->vram_address.reg += (self->control.increment_mode ? 32 : 1);
                break;
        }
    }

    return data;
}

void PPU::mainBusWrite(uint16_t address, uint8_t data) {
    switch (address) {
        case 0x000:
            control.reg = data;
            tram_address.nametable_x = control.nametable_x;
            tram_address.nametable_y = control.nametable_y;
            break;
        case 0x001:
            mask.reg = data;
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
    address &= 0x3FFF;  // PPU address bus wraps at 0x4000

    if (rom && rom->ppuBusRead(address, data)) {
        // Cartridge handles this address
    }
    else if (address >= 0x0000 && address <= 0x1FFF) {
        // Pattern tables (CHR ROM or RAM)
        data = patternTable[(address & 0x1000) >> 12][address & 0x0FFF];
    }
    else if (address >= 0x2000 && address <= 0x3EFF) {
        // Nametables (mirroring)
        address &= 0x0FFF;  // Mirror 4 screens into 4K
        uint16_t offset = address & 0x03FF;

        if (rom->mirror == ROM::Mirror::Vertical) {
            if (address < 0x0400)
                data = nameTable[0][offset];
            else if (address < 0x0800)
                data = nameTable[1][offset];
            else if (address < 0x0C00)
                data = nameTable[0][offset];
            else
                data = nameTable[1][offset];
        }
        else if (rom->mirror == ROM::Mirror::Horizontal) {
            if (address < 0x0800)
                data = nameTable[0][offset];
            else
                data = nameTable[1][offset];
        }
    }
    else if (address >= 0x3F00 && address <= 0x3FFF) {
        // Palette RAM indexes
        address &= 0x001F;

        // Handle palette mirroring for background colors
        if (address == 0x0010) address = 0x0000;
        if (address == 0x0014) address = 0x0004;
        if (address == 0x0018) address = 0x0008;
        if (address == 0x001C) address = 0x000C;

        data = paletteTable[address] & (mask.grayscale ? 0x30 : 0x3F);
    }

    // Default return 0x00 if no conditions matched
    return data;
}

void PPU::ppuBusWrite(uint16_t address, uint8_t data) {
    address &= 0x3FFF;

    if (rom && rom->ppuBusWrite(address, data)) {
        // Cartridge handles this write
    }
    else if (address >= 0x0000 && address <= 0x1FFF) {
        // Pattern tables (CHR ROM usually read-only, but can be RAM)
        patternTable[(address & 0x1000) >> 12][address & 0x0FFF] = data;
    }
    else if (address >= 0x2000 && address <= 0x3EFF) {
        // Nametables with mirroring
        address &= 0x0FFF;
        uint16_t offset = address & 0x03FF;

        if (rom->mirror == ROM::Mirror::Vertical) {
            if (address < 0x0400)
                nameTable[0][offset] = data;
            else if (address < 0x0800)
                nameTable[1][offset] = data;
            else if (address < 0x0C00)
                nameTable[0][offset] = data;
            else
                nameTable[1][offset] = data;
        }
        else if (rom->mirror == ROM::Mirror::Horizontal) {
            if (address < 0x0800)
                nameTable[0][offset] = data;
            else
                nameTable[1][offset] = data;
        }
    }
    else if (address >= 0x3F00 && address <= 0x3FFF) {
        // Palette RAM write with mirroring of background colors
        address &= 0x001F;

        if (address == 0x0010) address = 0x0000;
        if (address == 0x0014) address = 0x0004;
        if (address == 0x0018) address = 0x0008;
        if (address == 0x001C) address = 0x000C;

        paletteTable[address] = data;
    }
}



void PPU::attachRom(shared<ROM> rom) {
    this->rom = rom;
}

uint8_t PPU::GetPaletteIndex(uint8_t palette, uint8_t pixel)
{
    // Same logic as your GetColourFromPaletteRam but returning palette index
    uint16_t address = 0x3F00 + (palette << 2) + pixel;
    address &= 0x1F; // Mask to 32 bytes (palette RAM size)
    return paletteTable[address] & (mask.grayscale ? 0x30 : 0x3F);
}


void PPU::clock()
{
    // Lambda to increment scroll X (coarse X and nametable X)
    auto IncrementScrollX = [&]()
    {
        if (mask.render_background || mask.render_sprites)
        {
            if (vram_address.coarse_x == 31)
            {
                vram_address.coarse_x = 0;
                vram_address.nametable_x = ~vram_address.nametable_x & 0x01;
            }
            else
            {
                vram_address.coarse_x++;
            }
        }
    };

    // Lambda to increment scroll Y (fine Y, coarse Y, nametable Y)
    auto IncrementScrollY = [&]()
    {
        if (mask.render_background || mask.render_sprites)
        {
            if (vram_address.y_offset < 7)
            {
                vram_address.y_offset++;
            }
            else
            {
                vram_address.y_offset = 0;

                if (vram_address.coarse_y == 29)
                {
                    vram_address.coarse_y = 0;
                    vram_address.nametable_y = ~vram_address.nametable_y & 0x01;
                }
                else if (vram_address.coarse_y == 31)
                {
                    vram_address.coarse_y = 0;
                }
                else
                {
                    vram_address.coarse_y++;
                }
            }
        }
    };

    // Lambda to transfer horizontal bits from tram_address to vram_address
    auto TransferAddressX = [&]()
    {
        if (mask.render_background || mask.render_sprites)
        {
            vram_address.nametable_x = tram_address.nametable_x;
            vram_address.coarse_x = tram_address.coarse_x;
        }
    };

    // Lambda to transfer vertical bits from tram_address to vram_address
    auto TransferAddressY = [&]()
    {
        if (mask.render_background || mask.render_sprites)
        {
            vram_address.y_offset = tram_address.y_offset;
            vram_address.nametable_y = tram_address.nametable_y;
            vram_address.coarse_y = tram_address.coarse_y;
        }
    };

    // Lambda to load background shifters with next tile data
    auto LoadBackgroundShifters = [&]()
    {
        bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
        bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;

        bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
        bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
    };

    // Lambda to update (shift) shifters each cycle
    auto UpdateShifters = [&]()
    {
        if (mask.render_background)
        {
            bg_shifter_pattern_lo <<= 1;
            bg_shifter_pattern_hi <<= 1;

            bg_shifter_attrib_lo <<= 1;
            bg_shifter_attrib_hi <<= 1;
        }
    };

    // Visible scanlines and pre-render scanline (-1)
    if (scanline >= -1 && scanline < 240)
    {
        if (scanline == 0 && cycle == 0)
        {
            cycle = 1; // skip cycle 0 on odd frames
        }

        if (scanline == -1 && cycle == 1)
        {
            status.vertical_blank = 0; // clear vblank
            frameComplete = false;
        }

        if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338))
        {
            UpdateShifters();

            switch ((cycle - 1) % 8)
            {
                case 0:
                    LoadBackgroundShifters();

                    bg_next_tile_id = ppuBusRead(0x2000 | (vram_address.reg & 0x0FFF));
                    printf("Cycle %d: bg_next_tile_id read from 0x%04X = 0x%02X\n", cycle, 0x2000 | (vram_address.reg & 0x0FFF), bg_next_tile_id);
                    break;

                case 2:
                {
                    uint16_t attr_address = 0x23C0
                        | ((vram_address.nametable_y << 11) & 0x800)
                        | ((vram_address.nametable_x << 10) & 0x400)
                        | (((vram_address.coarse_y >> 2) << 3) & 0x38)
                        | ((vram_address.coarse_x >> 2) & 0x07);
                    bg_next_tile_attrib = ppuBusRead(attr_address);
                    printf("Cycle %d: bg_next_tile_attrib read from 0x%04X = 0x%02X\n", cycle, attr_address, bg_next_tile_attrib);

                    if (vram_address.coarse_y & 0x02) bg_next_tile_attrib >>= 4;
                    if (vram_address.coarse_x & 0x02) bg_next_tile_attrib >>= 2;
                    bg_next_tile_attrib &= 0x03;
                }
                break;

                case 4:
                    bg_next_tile_lsb = ppuBusRead(
                        (control.pattern_background << 12) + (bg_next_tile_id << 4) + vram_address.y_offset);
                    printf("Cycle %d: bg_next_tile_lsb read = 0x%02X\n", cycle, bg_next_tile_lsb);
                    break;

                case 6:
                    bg_next_tile_msb = ppuBusRead(
                        (control.pattern_background << 12) + (bg_next_tile_id << 4) + vram_address.y_offset + 8);
                    printf("Cycle %d: bg_next_tile_msb read = 0x%02X\n", cycle, bg_next_tile_msb);
                    break;

                case 7:
                    IncrementScrollX();
                    break;
            }
        }

        if (cycle == 256)
        {
            IncrementScrollY();
        }

        if (cycle == 257)
        {
            LoadBackgroundShifters();
            TransferAddressX();
        }

        if (cycle == 338 || cycle == 340)
        {
            bg_next_tile_id = ppuBusRead(0x2000 | (vram_address.reg & 0x0FFF));
            printf("Cycle %d: prefetch bg_next_tile_id read from 0x%04X = 0x%02X\n", cycle, 0x2000 | (vram_address.reg & 0x0FFF), bg_next_tile_id);
        }

        if (scanline == -1 && cycle >= 280 && cycle < 305)
        {
            TransferAddressY();
        }
    }

    if (scanline == 240)
    {
        // Post-render scanline - do nothing
    }

    if (scanline >= 241 && scanline < 261)
    {
        if (scanline == 241 && cycle == 1)
        {
            status.vertical_blank = 1;
            if (control.enable_nmi) nmi = true;
        }
    }

    // Compose background pixel
    uint8_t bg_pixel = 0x00;
    uint8_t bg_palette = 0x00;

    if (mask.render_background)
    {
        uint16_t bit_mux = 0x8000 >> x_offset;

        uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) ? 1 : 0;
        uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) ? 1 : 0;

        bg_pixel = (p1_pixel << 1) | p0_pixel;

        uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) ? 1 : 0;
        uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) ? 1 : 0;

        bg_palette = (bg_pal1 << 1) | bg_pal0;
    }

    // Write to framebuffer if in visible region
    if (scanline >= 0 && scanline < SCREEN_HEIGHT && cycle > 0 && cycle <= SCREEN_WIDTH)
    {
        uint8_t paletteIndex = GetPaletteIndex(bg_palette, bg_pixel);
        framebuffer[scanline * SCREEN_WIDTH + (cycle - 1)] = paletteIndex;

        static int pixelCount = 0;
        if (pixelCount % 1000 == 0)
        {
            printf("Scanline %d, Cycle %d, bg_pixel %d, bg_palette %d, PaletteIndex 0x%02X\n",
                scanline, cycle, bg_pixel, bg_palette, paletteIndex);
        }
        pixelCount++;
    }

    // Advance cycle & scanline counters
    cycle++;
    if (cycle >= 341)
    {
        cycle = 0;
        scanline++;
        if (scanline >= 261)
        {
            scanline = -1;
            frameComplete = true;
        }
    }
}


std::vector<uint32_t> PPU::getRGBFrameBuffer() const {
    static const uint32_t nesPalette[64] = {
        0xFF757575, 0xFF271B8F, 0xFF0000AB, 0xFF47009F, 0xFF8F0077, 0xFFAB0013, 0xFFA70000, 0xFF7F0B00,
        0xFF432F00, 0xFF004700, 0xFF005100, 0xFF003F17, 0xFF1B3F5F, 0xFF000000, 0xFF000000, 0xFF000000,
        0xFFBCBCBC, 0xFF0073EF, 0xFF233BEF, 0xFF8300F3, 0xFFBF00BF, 0xFFE7005B, 0xFFDB2B00, 0xFFCB4F0F,
        0xFF8B7300, 0xFF009700, 0xFF00AB00, 0xFF00933B, 0xFF00838B, 0xFF000000, 0xFF000000, 0xFF000000,
        0xFFFFFFFF, 0xFF3FBFFF, 0xFF5F97FF, 0xFFA078FF, 0xFFF878F8, 0xFFF85898, 0xFFF87858, 0xFFFFA05F,
        0xFFF0D87F, 0xFFB8F878, 0xFF58D854, 0xFF58F898, 0xFF00E8D8, 0xFF787878, 0xFF000000, 0xFF000000,
        0xFFFFFFFF, 0xFFA0FFC4, 0xFFB8F8F8, 0xFFF0D0FF, 0xFFFFC0FF, 0xFFFFC0CB, 0xFFFFD0B0, 0xFFFFE0A0,
        0xFFF0F090, 0xFFB0F0B0, 0xFF90CCC0, 0xFF90C0E0, 0xFF008080, 0xFF000000, 0xFF000000, 0xFF000000
    };

    std::vector<uint32_t> rgbPixels(SCREEN_WIDTH * SCREEN_HEIGHT);

    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        uint8_t index = framebuffer[i];
        // Clamp index just in case, NES palette has 64 colors
        if (index >= 64) index = 0;
        rgbPixels[i] = nesPalette[index];
    }
    return rgbPixels;
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


// For testing

void PPU::loadPatternTable(int table, int index, uint8_t value) {
    if (table >= 0 && table < 2 && index >= 0 && index < 4096)
        patternTable[table][index] = value;
}

void PPU::loadNameTable(int table, int index, uint8_t value) {
    if (table >= 0 && table < 2 && index >= 0 && index < 1024)
        nameTable[table][index] = value;
}

void PPU::loadPalette(int index, uint8_t value) {
    if (index >= 0 && index < 32)
        paletteTable[index] = value;
}

void PPU::setControlReg(uint8_t value) {
    control.reg = value;
}

void PPU::setMaskReg(uint8_t value) {
    mask.reg = value;
}

void PPU::setScanline(int16_t value) {
    scanline = value;
}

void PPU::setCycle(int16_t value) {
    cycle = value;
}

uint8_t PPU::getFramebufferPixel(int x, int y) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return 0;
    return framebuffer[y * SCREEN_WIDTH + x];
}
