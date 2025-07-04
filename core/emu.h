#pragma once

#include "./CPU/bus.h"
#include "ROM/rom.h"
#include <memory>

class Emulator {
public:
    Emulator();

    // Attach a ROM file (pass ownership/shared ptr)
    void attachRom(std::shared_ptr<ROM> rom);

    // Reset the emulator (CPU, PPU, etc)
    void reset();

    // Run a certain number of CPU cycles
    void runCycles(int cycles);

    // Run a single frame (roughly 29781 PPU cycles per NTSC frame)
    void runFrame();

    // Pause or resume emulation
    void pause();
    void resume();

    // Query running state
    bool isRunning() const;

    // Access Bus (e.g. for debugging)
    Bus& getBus() { return bus; }

    // read-only getters for GUI
    uint16_t pc()   const { return cpu.pc; }
    uint8_t  a()    const { return cpu.a;  }
    uint8_t  x()    const { return cpu.x;  }
    uint8_t  y()    const { return cpu.y;  }
    uint32_t cycles()const{ return bus.cyclesCounter; }

    Bus  bus;
    CPU& cpu = bus.cpu;       // alias
    PPU& ppu = bus.ppu;       // alias

    bool running = false;
};