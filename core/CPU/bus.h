#pragma once
#include <array>
#include <cstdint>

#include "cpu.h"
#include "../types.h"
#include "../PPU/ppu.h"
#include "../ROM/rom.h"

#ifdef BUILD_TEST
#define RAM_SIZE (0x10000)
#else
#define RAM_SIZE (2048)
#endif


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
    uint8_t controller[2];

    void write(uint16_t address, uint8_t data);
    uint8_t read(uint16_t address, bool readOnly = false);

    void attachRom(const shared<ROM> &rom);
    void reset();
    void tick();

    void setRunMode(RunMode mode) { runMode = mode; }
    RunMode getRunMode() const { return runMode; }

    uint32_t cyclesCounter;

private:
    RunMode runMode = RunMode::Running;
    // might want to expose this for frontend
    uint8_t controller_state[2];


// For testing - can consider ifdefs

public:
    void loadTestProgram(const std::string &path);
    void enableTestMode(bool enabled) { cpuTestMode = enabled; cpu.enableTrace(enabled); }
    bool isTestMode() const { return cpuTestMode; }
    std::vector<uint8_t> program;
private:
    bool cpuTestMode = false;

};
