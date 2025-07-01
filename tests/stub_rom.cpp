#include <cstdint>

#include "../core/ROM/rom.h"

bool ROM::mainBusRead(uint16_t, uint8_t&) {
    return false;
}