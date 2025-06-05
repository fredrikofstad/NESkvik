//
// NESkvikk - Fredrik Ofstad
//

#pragma once

#include <cstdint>

class CPU;

namespace address_mode {

    uint8_t IMP(CPU& cpu);  // Implied
    uint8_t IMM(CPU& cpu);  // Immediate
    uint8_t ZP0(CPU& cpu);  // Zero Page
    uint8_t ZPX(CPU& cpu);  // Zero Page,X
    uint8_t ZPY(CPU& cpu);  // Zero Page,Y
    uint8_t ABS(CPU& cpu);  // Absolute
    uint8_t ABX(CPU& cpu);  // Absolute,X
    uint8_t ABY(CPU& cpu);  // Absolute,Y
    uint8_t IND(CPU& cpu);  // Indirect
    uint8_t IZX(CPU& cpu);  // (Indirect,X)
    uint8_t IZY(CPU& cpu);  // (Indirect),Y
    uint8_t REL(CPU& cpu);  // Relative (for branches)

}