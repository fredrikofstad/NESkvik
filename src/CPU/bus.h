#pragma once
#include <array>
#include <cstdint>

#include "cpu.h"

#define RAM_SIZE (64 * 1024)

class bus {
public:
    bus();

    CPU cpu;
    std::array<uint8_t, RAM_SIZE> ram;


public:
    void write(uint16_t address, uint8_t data);
    uint8_t read(uint16_t address, bool readOnly = false) const;

};
