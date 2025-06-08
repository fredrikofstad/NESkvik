cmake_minimum_required(VERSION 3.31)
project(NESkvik)

set(CMAKE_CXX_STANDARD 20)

add_executable(NESkvik src/main.cpp
        src/CPU/Bus.cpp
        src/CPU/Bus.h
        src/CPU/CPU.cpp
        src/CPU/CPU.h
        src/Types.h
        src/CPU/opcodes.cpp
        src/CPU/opcodes.h
        src/CPU/address_modes.cpp
        src/CPU/address_modes.h
        src/CPU/lookup.h
        src/PPU/ppu.cpp
        src/PPU/ppu.h
        src/ROM/rom.cpp
        src/ROM/rom.h
        src/Mapper/mapper.cpp
        src/Mapper/mapper.h
        src/Mapper/mapper_0.cpp
        src/Mapper/mapper_0.h)
