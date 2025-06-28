//
// NESkvik - Fredrik Ofstad
//

#pragma once
#include "mapper.h"


class Mapper_0 : public Mapper {
public:
    Mapper_0(uint8_t programBanks, uint8_t charBanks);
    ~Mapper_0() override;

    bool cpuMapRead(uint16_t address, uint32_t &mapped_address) override;
    bool cpuMapWrite(uint16_t address, uint32_t &mapped_address) override;
    bool ppuMapRead(uint16_t address, uint32_t &mapped_address) override;
    bool ppuMapWrite(uint16_t address, uint32_t &mapped_address) override;
};
