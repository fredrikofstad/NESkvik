#include "bus.h"

#include <algorithm>
#include <fstream>
#include <sstream>

Bus::Bus() {
    // Clear RAM on startup
    for (auto &i : ram) i = 0x00;

    cpu.AttachBus(this);
}

bool isSystemRam(const uint16_t address) {
    return address <= 0x0000 || address >= 0x1FFF;
}
bool isPPURam(const uint16_t address) {
    return address <= 0x2000 || address >= 0x3FFF;
}

uint8_t Bus::read(uint16_t address, bool readOnly) const {
    // test for CPU tests
    if (cpuTestMode) {
        if (address >= 0x8000 && !program.empty()) {
            size_t offset = address - 0x8000;
            printf("Bus::read from program offset=%04X val=%02X\n", offset, program[offset]);
            return program[offset];
        }
        printf("Bus::read TESTMODE addr=%04X val=%02X\n", address, ram[address]);
        return ram[address];
    }

    // NES mode:
    uint8_t data = 0x00;

    // normal NES RAM (2KB mirrored)
    if (address <= 0x1FFF) {
        data = ram[address & 0x07FF];
    }
    // PPU registers mirrored every 8 bytes
    else if (address >= 0x2000 && address <= 0x3FFF) {
        data = ppu.mainBusRead(address & 0x0007, readOnly);
    }
    // PRG-ROM through mapper/cartridge
    else if (rom && rom->mainBusRead(address, data)) {
        return data;
    }
    return data;
}


void Bus::write(uint16_t address, uint8_t data) {
    if (cpuTestMode) {
        // 64KB flat RAM for CPU test
        ram[address] = data;
        return;
    }

    // NES mode
    if (address <= 0x1FFF) {
        ram[address & 0x07FF] = data;
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        ppu.mainBusWrite(address & 0x0007, data);
    }
    // PRG-ROM writes typically ignored or handled by mapper
}
void Bus::attachRom(const shared<ROM> &rom) {
    this->rom = rom;
    ppu.attachRom(rom);
}

void Bus::reset() {
    cpu.reset();
    cyclesCounter = 0;
}

void Bus::tick() {
    if (runMode == RunMode::Paused) {
        return;
    }

    if (!cpuTestMode)
        ppu.clock();
    if (cyclesCounter % 3 == 0) {
        cpu.clock();
    }
    cyclesCounter++;

    if (runMode == RunMode::Step) {
        runMode = RunMode::Paused;
    }
}


// Cpu tests
void Bus::loadTestProgram(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        printf("could not open file\n");
        return;
    }

    std::vector<uint8_t> testbin(std::istreambuf_iterator<char>(in), {});
    printf("testbin size = %zu\n", testbin.size());

    // patch program vector for reset vector
    program.clear();
    program.resize(0x8000, 0xFF);
    program[0xFFFC - 0x8000] = 0x00;
    program[0xFFFD - 0x8000] = 0x04;

    // copy entire test binary into 64K RAM
    for (size_t i = 0; i < testbin.size(); ++i) {
        ram[i] = testbin[i];
    }
    printf("RAM[0x0400] = %02X\n", ram[0x0400]);
    for (size_t i = 0; i < testbin.size(); ++i) {
        ram[i] = testbin[i];
    }
}