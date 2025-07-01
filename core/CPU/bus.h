#pragma once
#include <array>
#include <cstdint>

#include "cpu.h"
#include "../types.h"
#include "../PPU/ppu.h"
#include "../ROM/rom.h"

//#define RAM_SIZE (2048)

// for cpu testing
#define RAM_SIZE (0x10000)

enum class RunMode {
    Paused,
    Running,
    Step
};


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

    void setRunMode(RunMode mode) { runMode = mode; }
    RunMode getRunMode() const { return runMode; }

    uint32_t cyclesCounter;

private:
    RunMode runMode = RunMode::Running;


// For testing
public:
    void loadTestProgram(const std::string &path);
    void enableTestMode(bool enabled) { cpuTestMode = enabled; cpu.enableTrace(enabled); }
    bool isTestMode() const { return cpuTestMode; }
    std::vector<uint8_t> program;
private:
    bool cpuTestMode = false;

};
