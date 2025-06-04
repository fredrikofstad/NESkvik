//
// Emulation of a 6502 cpu
//
#pragma once
#include "bus.h"


class CPU {
public:
    CPU();

    void AttachBus(bus* bus) {
        this->bus = bus;
    }

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

    // Addressing Modes
    uint8_t IMP();
    uint8_t IMM();
    uint8_t ZPO();
    uint8_t ZPX();
    uint8_t ZPY();
    uint8_t REL();
    uint8_t ABS();
    uint8_t ABX();
    uint8_t ABY();
    uint8_t IND();
    uint8_t IZX();
    uint8_t IZY();

    // 6502 CPU Registers
    uint8_t a = 0x00;        // Accumulator
    uint8_t x = 0x00;        // X index register
    uint8_t y = 0x00;        // Y index register
    uint16_t pc = 0x0000;    // Program counter
    uint8_t sp = 0x00;       // Stack pointer
    uint8_t status = 0x00;   // Processor status flags

private:
    bus* bus = nullptr;
    uint8_t read(uint8_t address) const;
    void write(uint8_t address, uint8_t data);

    uint8_t getFlag(flags flag);
    void setFlag(flags flag, bool value);

};

