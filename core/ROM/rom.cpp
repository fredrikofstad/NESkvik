//
// NESkvikk - Fredrik Ofstad
//

#include "rom.h"

#include <cstdint>
#include <fstream>

#define PRG_BANK_SIZE 16384
#define CHR_BANK_SIZE 8192


ROM::ROM(const std::string &filename) {
    struct Header {
        char name[4];
        uint8_t prg_chunks;
        uint8_t chr_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header;

    std::ifstream ifstream;
    ifstream.open(filename, std::ifstream::binary);
    if (ifstream.is_open()) {
        // read header
        ifstream.read(reinterpret_cast<char *>(&header), sizeof(Header));
        if (header.mapper1 & 0x04)
            ifstream.seekg(512, std::ios_base::cur);
        // determine mapper id
        mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
        // determine file format
        uint8_t fileType = 1;
        if (fileType == 1) {
            program_banks = header.prg_chunks;
            program_memory.resize(program_banks * PRG_BANK_SIZE);
            ifstream.read(reinterpret_cast<char *>(program_memory.data()), program_memory.size());

            char_banks = header.chr_chunks;
            char_memory.resize(char_banks * CHR_BANK_SIZE);
            ifstream.read(reinterpret_cast<char *>(char_memory.data()), char_memory.size());
        }

        switch (mapperID) {
            case 0:
                mapper = std::make_shared<Mapper_0>(program_banks, char_banks);
                break;
            default: break;
        }

        ifstream.close();
    }
}

bool ROM::mainBusRead(uint16_t address, uint8_t &data) {
    if (uint32_t mapped_address = 0; mapper->cpuMapRead(address, mapped_address)){
        data = program_memory[mapped_address];
        return true;
    }
    return false;
}

bool ROM::mainBusWrite(uint16_t address, uint8_t data) {
    if (uint32_t mapped_address = 0; mapper->cpuMapRead(address, mapped_address)){
        program_memory[mapped_address] = data;
        return true;
    }
    return false;
}

bool ROM::ppuBusRead(uint16_t address, uint8_t &data) {
    if (uint32_t mapped_address = 0; mapper->ppuMapRead(address, mapped_address)){
        data = char_memory[mapped_address];
        return true;
    }
    return false;
}

bool ROM::ppuBusWrite(uint16_t address, uint8_t data) {
    if (uint32_t mapped_address = 0; mapper->ppuMapRead(address, mapped_address)){
        char_memory[mapped_address] = data;
        return true;
    }
    return false;
}