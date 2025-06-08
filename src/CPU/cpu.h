//
// Emulation of a 6502 cpu
//
#pragma once
#include <string>
#include <vector>

#include "bus.h"

namespace flags {
    enum flags {
        Carry = (1 << 0),      // sets the carry flag
        Zero = (1 << 1),       // sets result as zero
        Interrupts = (1 << 2), // disables interrupts
        Decimal = (1 << 3),    // sets decimal mode (Not applicable for NES)
        Break = (1 << 4),      // for break operations
        Unused = (1 << 5),     // unused bit
        Overflow = (1 << 6),   // sets overflow flag
        Negative = (1 << 7),   // sets negative for signed vars
    };
}


class CPU {
public:
    CPU();

    void AttachBus(bus* bus) {
        this->bus = bus;
    }


    // 6502 CPU Registers
    uint8_t a = 0x00;        // Accumulator
    uint8_t x = 0x00;        // X index register
    uint8_t y = 0x00;        // Y index register
    uint16_t pc = 0x0000;    // Program counter
    uint8_t sp = 0x00;       // Stack pointer
    uint8_t status = 0x00;   // Processor status flags

    void clock();  // clock signal
    void reset();  // request signal
    void irq();    // interrupt request signal
    void nmi();    // non-maskable interrupt signal

    uint8_t read(uint8_t address) const;
    void write(uint8_t address, uint8_t data);

    uint8_t getFlag(flags flag);
    void setFlag(flags flag, bool value);

    uint8_t fetch();

    void branchIf(bool condition);
    void compare(uint8_t reg);
    uint8_t load(uint8_t reg);

    uint8_t fetched_data = 0x00;
    uint8_t store = 0x000;
    uint16_t address_abs = 0x0000;
    uint16_t address_rel = 0x00;
    uint8_t current_opcode = 0x00;
    uint8_t cycles = 0;

private:
    bus* bus = nullptr;

};

