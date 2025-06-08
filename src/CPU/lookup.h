//
// NESkvikk - Fredrik Ofstad
//

#pragma once

#include <string>
#include <tuple>
#include <array>
#include <cstdint>
#include <functional>

#include "cpu.h"

class CPU;

namespace opcodes {
    using OpFunc = uint8_t(*)(CPU&);
}

namespace address_mode {
    using AddrFunc = uint8_t(*)(CPU&);
    bool checkAddressMode(uint8_t(*)(CPU&));
}

using Instruction = std::tuple<std::string, opcodes::OpFunc, address_mode::AddrFunc, uint8_t>;

// There are 256 possible opcodes (0x00 to 0xFF)
extern std::array<Instruction, 256> lookup;

inline bool isAddressMode(const CPU& cpu, address_mode::AddrFunc mode) {
    return std::get<2>(lookup[cpu.current_opcode]) == mode;
}