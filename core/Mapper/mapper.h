//
// NESkvik - Fredrik Ofstad
//

#pragma once
#include <cstdint>


class Mapper {

public:
    Mapper(uint8_t programBanks, uint8_t charBanks);
    virtual ~Mapper() = default;

    virtual bool cpuMapRead(uint16_t address, uint32_t &mapped_address) = 0;
    virtual bool cpuMapWrite(uint16_t address, uint32_t &mapped_address) = 0;
    virtual bool ppuMapRead(uint16_t address, uint32_t &mapped_address) = 0;
    virtual bool ppuMapWrite(uint16_t address, uint32_t &mapped_address) = 0;

protected:
    uint8_t programBanks = 0;
    uint8_t charBanks = 0;
};
