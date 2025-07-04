//
// NESkvik - Fredrik Ofstad
//
#include "emu.h"

Emulator::Emulator() : running(false) {}

void Emulator::attachRom(std::shared_ptr<ROM> rom) {
    bus.attachRom(rom);
}

void Emulator::reset() {
    bus.reset();
    bus.setRunMode(RunMode::Paused);
}

void Emulator::runCycles(int cycles) {
    bus.setRunMode(RunMode::Running);
    for (int i = 0; i < cycles; ++i) {
        bus.tick();
    }
}

void Emulator::runFrame() {
    // NES NTSC PPU runs at ~29780 cycles per frame
    constexpr int cyclesPerFrame = 29781;
    runCycles(cyclesPerFrame);
}

void Emulator::pause() {
    bus.setRunMode(RunMode::Paused);  // Ensure bus respects pause mode
}

void Emulator::resume() {
    bus.setRunMode(RunMode::Running); // Make sure bus is running
}

bool Emulator::isRunning() const {
    return running;
}
