//
// NESkvikk - Fredrik Ofstad
//

#pragma once

#include <string>
#include <tuple>
#include <array>
#include <cstdint>
#include <vector>

#include "cpu.h"

class CPU;

namespace opcodes {
    using OpFunc = uint8_t(*)(CPU&);
}

namespace address_mode {
    using AddrFunc = uint8_t(*)(CPU&);
    bool checkAddressMode(uint8_t(*)(CPU&));
}


namespace opcodes {
    using OpFunc = uint8_t(*)(CPU&);
}

namespace address_mode {
    using AddrFunc = uint8_t(*)(CPU&);
}

struct INSTRUCTION {
    std::string name;
    opcodes::OpFunc opcode = nullptr;
    address_mode::AddrFunc addressmode = nullptr;
    uint8_t cycles = 0;
};

// There are 256 possible opcodes (0x00 to 0xFF)
extern std::vector<INSTRUCTION> lookup;

inline bool isAddressMode(const CPU& cpu, address_mode::AddrFunc mode) {
    return lookup[cpu.current_opcode].addressmode == mode;
}