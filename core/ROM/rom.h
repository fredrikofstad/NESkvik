//
// NESkvik - Fredrik Ofstad
//

#pragma once
#include <string>
#include <vector>
#include "../types.h"

#include "../Mapper/mapper_0.h"


class ROM {
public:
    explicit ROM(const std::string& filename);

    bool mainBusRead(uint16_t address, uint8_t &data);
    bool mainBusWrite(uint16_t address, uint8_t data);

    bool ppuBusRead(uint16_t address, uint8_t &data);
    bool ppuBusWrite(uint16_t address, uint8_t data);

private:
    std::vector<uint8_t> program_memory;
    std::vector<uint8_t> char_memory;

    shared<Mapper> mapper;

    uint8_t mapperID = 0;
    uint8_t program_banks = 0;
    uint8_t char_banks = 0;

};
