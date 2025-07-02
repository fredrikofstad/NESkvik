#include "lookup.h"
#include "address_modes.h"
#include "opcodes.h"


std::vector<INSTRUCTION> lookup = {

 // 0x00
    { "BRK", &opcodes::BRK, &address_mode::IMM, 7 },      // 00
    { "ORA", &opcodes::ORA, &address_mode::IZX, 6 },      // 01
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 02
    { "???", &opcodes::XXX, &address_mode::IMP, 8 },      // 03
    { "NOP", &opcodes::NOP, &address_mode::IMP, 3 },      // 04
    { "ORA", &opcodes::ORA, &address_mode::ZP0, 3 },      // 05
    { "ASL", &opcodes::ASL, &address_mode::ZP0, 5 },      // 06
    { "???", &opcodes::XXX, &address_mode::IMP, 5 },      // 07
    { "PHP", &opcodes::PHP, &address_mode::IMP, 3 },      // 08
    { "ORA", &opcodes::ORA, &address_mode::IMM, 2 },      // 09
    { "ASL", &opcodes::ASL, &address_mode::IMP, 2 },      // 0A
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 0B
    { "NOP", &opcodes::NOP, &address_mode::IMP, 4 },      // 0C
    { "ORA", &opcodes::ORA, &address_mode::ABS, 4 },      // 0D
    { "ASL", &opcodes::ASL, &address_mode::ABS, 6 },      // 0E
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // 0F

    // 0x10
    { "BPL", &opcodes::BPL, &address_mode::REL, 2 },      // 10
    { "ORA", &opcodes::ORA, &address_mode::IZY, 5 },      // 11
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 12
    { "???", &opcodes::XXX, &address_mode::IMP, 8 },      // 13
    { "NOP", &opcodes::NOP, &address_mode::IMP, 4 },      // 14
    { "ORA", &opcodes::ORA, &address_mode::ZPX, 4 },      // 15
    { "ASL", &opcodes::ASL, &address_mode::ZPX, 6 },      // 16
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // 17
    { "CLC", &opcodes::CLC, &address_mode::IMP, 2 },      // 18
    { "ORA", &opcodes::ORA, &address_mode::ABY, 4 },      // 19
    { "???", &opcodes::NOP, &address_mode::IMP, 2 },      // 1A
    { "???", &opcodes::XXX, &address_mode::IMP, 7 },      // 1B
    { "NOP", &opcodes::NOP, &address_mode::IMP, 4 },      // 1C
    { "ORA", &opcodes::ORA, &address_mode::ABX, 4 },      // 1D
    { "ASL", &opcodes::ASL, &address_mode::ABX, 7 },      // 1E
    { "???", &opcodes::XXX, &address_mode::IMP, 7 },      // 1F

    // 0x20
    { "JSR", &opcodes::JSR, &address_mode::ABS, 6 },      // 20
    { "AND", &opcodes::AND, &address_mode::IZX, 6 },      // 21
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 22
    { "???", &opcodes::XXX, &address_mode::IMP, 8 },      // 23
    { "BIT", &opcodes::BIT, &address_mode::ZP0, 3 },      // 24
    { "AND", &opcodes::AND, &address_mode::ZP0, 3 },      // 25
    { "ROL", &opcodes::ROL, &address_mode::ZP0, 5 },      // 26
    { "???", &opcodes::XXX, &address_mode::IMP, 5 },      // 27
    { "PLP", &opcodes::PLP, &address_mode::IMP, 4 },      // 28
    { "AND", &opcodes::AND, &address_mode::IMM, 2 },      // 29
    { "ROL", &opcodes::ROL, &address_mode::IMP, 2 },      // 2A
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 2B
    { "BIT", &opcodes::BIT, &address_mode::ABS, 4 },      // 2C
    { "AND", &opcodes::AND, &address_mode::ABS, 4 },      // 2D
    { "ROL", &opcodes::ROL, &address_mode::ABS, 6 },      // 2E
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // 2F

    // 0x30
    { "BMI", &opcodes::BMI, &address_mode::REL, 2 },      // 30
    { "AND", &opcodes::AND, &address_mode::IZY, 5 },      // 31
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 32
    { "???", &opcodes::XXX, &address_mode::IMP, 8 },      // 33
    { "NOP", &opcodes::NOP, &address_mode::IMP, 4 },      // 34
    { "AND", &opcodes::AND, &address_mode::ZPX, 4 },      // 35
    { "ROL", &opcodes::ROL, &address_mode::ZPX, 6 },      // 36
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // 37
    { "SEC", &opcodes::SEC, &address_mode::IMP, 2 },      // 38
    { "AND", &opcodes::AND, &address_mode::ABY, 4 },      // 39
    { "???", &opcodes::NOP, &address_mode::IMP, 2 },      // 3A
    { "???", &opcodes::XXX, &address_mode::IMP, 7 },      // 3B
    { "NOP", &opcodes::NOP, &address_mode::IMP, 4 },      // 3C
    { "AND", &opcodes::AND, &address_mode::ABX, 4 },      // 3D
    { "ROL", &opcodes::ROL, &address_mode::ABX, 7 },      // 3E
    { "???", &opcodes::XXX, &address_mode::IMP, 7 },      // 3F

    // 0x40
    { "RTI", &opcodes::RTI, &address_mode::IMP, 6 },      // 40
    { "EOR", &opcodes::EOR, &address_mode::IZX, 6 },      // 41
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 42
    { "???", &opcodes::XXX, &address_mode::IMP, 8 },      // 43
    { "NOP", &opcodes::NOP, &address_mode::IMP, 3 },      // 44
    { "EOR", &opcodes::EOR, &address_mode::ZP0, 3 },      // 45
    { "LSR", &opcodes::LSR, &address_mode::ZP0, 5 },      // 46
    { "???", &opcodes::XXX, &address_mode::IMP, 5 },      // 47
    { "PHA", &opcodes::PHA, &address_mode::IMP, 3 },      // 48
    { "EOR", &opcodes::EOR, &address_mode::IMM, 2 },      // 49
    { "LSR", &opcodes::LSR, &address_mode::IMP, 2 },      // 4A
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 4B
    { "JMP", &opcodes::JMP, &address_mode::ABS, 3 },      // 4C
    { "EOR", &opcodes::EOR, &address_mode::ABS, 4 },      // 4D
    { "LSR", &opcodes::LSR, &address_mode::ABS, 6 },      // 4E
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // 4F

    // 0x50
    { "BVC", &opcodes::BVC, &address_mode::REL, 2 },      // 50
    { "EOR", &opcodes::EOR, &address_mode::IZY, 5 },      // 51
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 52
    { "???", &opcodes::XXX, &address_mode::IMP, 8 },      // 53
    { "NOP", &opcodes::NOP, &address_mode::IMP, 4 },      // 54
    { "EOR", &opcodes::EOR, &address_mode::ZPX, 4 },      // 55
    { "LSR", &opcodes::LSR, &address_mode::ZPX, 6 },      // 56
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // 57
    { "CLI", &opcodes::CLI, &address_mode::IMP, 2 },      // 58
    { "EOR", &opcodes::EOR, &address_mode::ABY, 4 },      // 59
    { "???", &opcodes::NOP, &address_mode::IMP, 2 },      // 5A
    { "???", &opcodes::XXX, &address_mode::IMP, 7 },      // 5B
    { "NOP", &opcodes::NOP, &address_mode::IMP, 4 },      // 5C
    { "EOR", &opcodes::EOR, &address_mode::ABX, 4 },      // 5D
    { "LSR", &opcodes::LSR, &address_mode::ABX, 7 },      // 5E
    { "???", &opcodes::XXX, &address_mode::IMP, 7 },      // 5F

    // 0x60
    { "RTS", &opcodes::RTS, &address_mode::IMP, 6 },      // 60
    { "ADC", &opcodes::ADC, &address_mode::IZX, 6 },      // 61
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 62
    { "???", &opcodes::XXX, &address_mode::IMP, 8 },      // 63
    { "NOP", &opcodes::NOP, &address_mode::IMP, 3 },      // 64
    { "ADC", &opcodes::ADC, &address_mode::ZP0, 3 },      // 65
    { "ROR", &opcodes::ROR, &address_mode::ZP0, 5 },      // 66
    { "???", &opcodes::XXX, &address_mode::IMP, 5 },      // 67
    { "PLA", &opcodes::PLA, &address_mode::IMP, 4 },      // 68
    { "ADC", &opcodes::ADC, &address_mode::IMM, 2 },      // 69
    { "ROR", &opcodes::ROR, &address_mode::IMP, 2 },      // 6A
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 6B
    { "JMP", &opcodes::JMP, &address_mode::IND, 5 },      // 6C
    { "ADC", &opcodes::ADC, &address_mode::ABS, 4 },      // 6D
    { "ROR", &opcodes::ROR, &address_mode::ABS, 6 },      // 6E
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // 6F

    // 0x70
    { "BVS", &opcodes::BVS, &address_mode::REL, 2 },      // 70
    { "ADC", &opcodes::ADC, &address_mode::IZY, 5 },      // 71
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 72
    { "???", &opcodes::XXX, &address_mode::IMP, 8 },      // 73
    { "NOP", &opcodes::NOP, &address_mode::IMP, 4 },      // 74
    { "ADC", &opcodes::ADC, &address_mode::ZPX, 4 },      // 75
    { "ROR", &opcodes::ROR, &address_mode::ZPX, 6 },      // 76
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // 77
    { "SEI", &opcodes::SEI, &address_mode::IMP, 2 },      // 78
    { "ADC", &opcodes::ADC, &address_mode::ABY, 4 },      // 79
    { "???", &opcodes::NOP, &address_mode::IMP, 2 },      // 7A
    { "???", &opcodes::XXX, &address_mode::IMP, 7 },      // 7B
    { "NOP", &opcodes::NOP, &address_mode::IMP, 4 },      // 7C
    { "ADC", &opcodes::ADC, &address_mode::ABX, 4 },      // 7D
    { "ROR", &opcodes::ROR, &address_mode::ABX, 7 },      // 7E
    { "???", &opcodes::XXX, &address_mode::IMP, 7 },      // 7F

    // 0x80
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 80
    { "STA", &opcodes::STA, &address_mode::IZX, 6 },      // 81
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 82
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // 83
    { "STY", &opcodes::STY, &address_mode::ZP0, 3 },      // 84
    { "STA", &opcodes::STA, &address_mode::ZP0, 3 },      // 85
    { "STX", &opcodes::STX, &address_mode::ZP0, 3 },      // 86
    { "???", &opcodes::XXX, &address_mode::IMP, 3 },      // 87
    { "DEY", &opcodes::DEY, &address_mode::IMP, 2 },      // 88
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 89
    { "TXA", &opcodes::TXA, &address_mode::IMP, 2 },      // 8A
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 8B
    { "STY", &opcodes::STY, &address_mode::ABS, 4 },      // 8C
    { "STA", &opcodes::STA, &address_mode::ABS, 4 },      // 8D
    { "STX", &opcodes::STX, &address_mode::ABS, 4 },      // 8E
    { "???", &opcodes::XXX, &address_mode::IMP, 4 },      // 8F

    // 0x90
    { "BCC", &opcodes::BCC, &address_mode::REL, 2 },      // 90
    { "STA", &opcodes::STA, &address_mode::IZY, 6 },      // 91
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // 92
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // 93
    { "STY", &opcodes::STY, &address_mode::ZPX, 4 },      // 94
    { "STA", &opcodes::STA, &address_mode::ZPX, 4 },      // 95
    { "STX", &opcodes::STX, &address_mode::ZPY, 4 },      // 96
    { "???", &opcodes::XXX, &address_mode::IMP, 4 },      // 97
    { "TYA", &opcodes::TYA, &address_mode::IMP, 2 },      // 98
    { "STA", &opcodes::STA, &address_mode::ABY, 5 },      // 99
    { "TXS", &opcodes::TXS, &address_mode::IMP, 2 },      // 9A
    { "???", &opcodes::XXX, &address_mode::IMP, 5 },      // 9B
    { "???", &opcodes::NOP, &address_mode::IMP, 5 },      // 9C
    { "STA", &opcodes::STA, &address_mode::ABX, 5 },      // 9D
    { "???", &opcodes::XXX, &address_mode::IMP, 5 },      // 9E
    { "???", &opcodes::XXX, &address_mode::IMP, 5 },      // 9F

    // 0xA0
    { "LDY", &opcodes::LDY, &address_mode::IMM, 2 },      // A0
    { "LDA", &opcodes::LDA, &address_mode::IZX, 6 },      // A1
    { "LDX", &opcodes::LDX, &address_mode::IMM, 2 },      // A2
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // A3
    { "LDY", &opcodes::LDY, &address_mode::ZP0, 3 },      // A4
    { "LDA", &opcodes::LDA, &address_mode::ZP0, 3 },      // A5
    { "LDX", &opcodes::LDX, &address_mode::ZP0, 3 },      // A6
    { "???", &opcodes::XXX, &address_mode::IMP, 3 },      // A7
    { "TAY", &opcodes::TAY, &address_mode::IMP, 2 },      // A8
    { "LDA", &opcodes::LDA, &address_mode::IMM, 2 },      // A9
    { "TAX", &opcodes::TAX, &address_mode::IMP, 2 },      // AA
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // AB
    { "LDY", &opcodes::LDY, &address_mode::ABS, 4 },      // AC
    { "LDA", &opcodes::LDA, &address_mode::ABS, 4 },      // AD
    { "LDX", &opcodes::LDX, &address_mode::ABS, 4 },      // AE
    { "???", &opcodes::XXX, &address_mode::IMP, 4 },      // AF

    // 0xB0
    { "BCS", &opcodes::BCS, &address_mode::REL, 2 },      // B0
    { "LDA", &opcodes::LDA, &address_mode::IZY, 5 },      // B1
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // B2
    { "???", &opcodes::XXX, &address_mode::IMP, 5 },      // B3
    { "LDY", &opcodes::LDY, &address_mode::ZPX, 4 },      // B4
    { "LDA", &opcodes::LDA, &address_mode::ZPX, 4 },      // B5
    { "LDX", &opcodes::LDX, &address_mode::ZPY, 4 },      // B6
    { "???", &opcodes::XXX, &address_mode::IMP, 4 },      // B7
    { "CLV", &opcodes::CLV, &address_mode::IMP, 2 },      // B8
    { "LDA", &opcodes::LDA, &address_mode::ABY, 4 },      // B9
    { "TSX", &opcodes::TSX, &address_mode::IMP, 2 },      // BA
    { "???", &opcodes::XXX, &address_mode::IMP, 4 },      // BB
    { "LDY", &opcodes::LDY, &address_mode::ABX, 4 },      // BC
    { "LDA", &opcodes::LDA, &address_mode::ABX, 4 },      // BD
    { "LDX", &opcodes::LDX, &address_mode::ABY, 4 },      // BE
    { "???", &opcodes::XXX, &address_mode::IMP, 4 },      // BF

    // 0xC0
    { "CPY", &opcodes::CPY, &address_mode::IMM, 2 },      // C0
    { "CMP", &opcodes::CMP, &address_mode::IZX, 6 },      // C1
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // C2
    { "???", &opcodes::XXX, &address_mode::IMP, 8 },      // C3
    { "CPY", &opcodes::CPY, &address_mode::ZP0, 3 },      // C4
    { "CMP", &opcodes::CMP, &address_mode::ZP0, 3 },      // C5
    { "DEC", &opcodes::DEC, &address_mode::ZP0, 5 },      // C6
    { "???", &opcodes::XXX, &address_mode::IMP, 5 },      // C7
    { "INY", &opcodes::INY, &address_mode::IMP, 2 },      // C8
    { "CMP", &opcodes::CMP, &address_mode::IMM, 2 },      // C9
    { "DEX", &opcodes::DEX, &address_mode::IMP, 2 },      // CA
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // CB
    { "CPY", &opcodes::CPY, &address_mode::ABS, 4 },      // CC
    { "CMP", &opcodes::CMP, &address_mode::ABS, 4 },      // CD
    { "DEC", &opcodes::DEC, &address_mode::ABS, 6 },      // CE
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // CF

    // 0xD0
    { "BNE", &opcodes::BNE, &address_mode::REL, 2 },      // D0
    { "CMP", &opcodes::CMP, &address_mode::IZY, 5 },      // D1
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // D2
    { "???", &opcodes::XXX, &address_mode::IMP, 8 },      // D3
    { "???", &opcodes::NOP, &address_mode::IMP, 4 },      // D4
    { "CMP", &opcodes::CMP, &address_mode::ZPX, 4 },      // D5
    { "DEC", &opcodes::DEC, &address_mode::ZPX, 6 },      // D6
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // D7
    { "CLD", &opcodes::CLD, &address_mode::IMP, 2 },      // D8
    { "CMP", &opcodes::CMP, &address_mode::ABY, 4 },      // D9
    { "???", &opcodes::NOP, &address_mode::IMP, 2 },      // DA
    { "???", &opcodes::XXX, &address_mode::IMP, 7 },      // DB
    { "???", &opcodes::NOP, &address_mode::IMP, 4 },      // DC
    { "CMP", &opcodes::CMP, &address_mode::ABX, 4 },      // DD
    { "DEC", &opcodes::DEC, &address_mode::ABX, 7 },      // DE
    { "???", &opcodes::XXX, &address_mode::IMP, 7 },      // DF

        // 0xE0
    { "CPX", &opcodes::CPX, &address_mode::IMM, 2 },      // E0
    { "SBC", &opcodes::SBC, &address_mode::IZX, 6 },      // E1
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // E2
    { "???", &opcodes::XXX, &address_mode::IMP, 8 },      // E3
    { "CPX", &opcodes::CPX, &address_mode::ZP0, 3 },      // E4
    { "SBC", &opcodes::SBC, &address_mode::ZP0, 3 },      // E5
    { "INC", &opcodes::INC, &address_mode::ZP0, 5 },      // E6
    { "???", &opcodes::XXX, &address_mode::IMP, 5 },      // E7
    { "INX", &opcodes::INX, &address_mode::IMP, 2 },      // E8
    { "SBC", &opcodes::SBC, &address_mode::IMM, 2 },      // E9
    { "NOP", &opcodes::NOP, &address_mode::IMP, 2 },      // EA
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // EB
    { "CPX", &opcodes::CPX, &address_mode::ABS, 4 },      // EC
    { "SBC", &opcodes::SBC, &address_mode::ABS, 4 },      // ED
    { "INC", &opcodes::INC, &address_mode::ABS, 6 },      // EE
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // EF

    // 0xF0
    { "BEQ", &opcodes::BEQ, &address_mode::REL, 2 },      // F0
    { "SBC", &opcodes::SBC, &address_mode::IZY, 5 },      // F1
    { "???", &opcodes::XXX, &address_mode::IMP, 2 },      // F2
    { "???", &opcodes::XXX, &address_mode::IMP, 8 },      // F3
    { "???", &opcodes::NOP, &address_mode::IMP, 4 },      // F4
    { "SBC", &opcodes::SBC, &address_mode::ZPX, 4 },      // F5
    { "INC", &opcodes::INC, &address_mode::ZPX, 6 },      // F6
    { "???", &opcodes::XXX, &address_mode::IMP, 6 },      // F7
    { "SED", &opcodes::SED, &address_mode::IMP, 2 },      // F8
    { "SBC", &opcodes::SBC, &address_mode::ABY, 4 },      // F9
    { "???", &opcodes::NOP, &address_mode::IMP, 2 },      // FA
    { "???", &opcodes::XXX, &address_mode::IMP, 7 },      // FB
    { "???", &opcodes::NOP, &address_mode::IMP, 4 },      // FC
    { "SBC", &opcodes::SBC, &address_mode::ABX, 4 },      // FD
    { "INC", &opcodes::INC, &address_mode::ABX, 7 },      // FE
    { "???", &opcodes::XXX, &address_mode::IMP, 7 },      // FF
};