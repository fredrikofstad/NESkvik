cmake_minimum_required(VERSION 3.31)
project(NESkvik)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

# optionally build web or desktop
option(BUILD_WEB "Build the web version via Emscripten" OFF)
option(BUILD_DESKTOP "Build the native desktop GUI" OFF)
option(BUILD_TEST "Build the test version" OFF)

# NESkvik core library
add_library(NESkvik STATIC
        core/CPU/bus.cpp
        core/CPU/bus.h
        core/CPU/cpu.cpp
        core/CPU/cpu.h
        core/types.h
        core/CPU/opcodes.cpp
        core/CPU/opcodes.h
        core/CPU/address_modes.cpp
        core/CPU/address_modes.h
        core/CPU/lookup.cpp
        core/CPU/lookup.h
        core/PPU/ppu.cpp
        core/PPU/ppu.h
        core/ROM/rom.cpp
        core/ROM/rom.h
        core/Mapper/mapper.cpp
        core/Mapper/mapper.h
        core/Mapper/mapper_0.cpp
        core/Mapper/mapper_0.h
        core/emu.h
        core/PPU/ppu_registers.h
)

# Desktop Build
if(BUILD_DESKTOP)
   add_subdirectory(desktop)
endif()

# Web Build
if(BUILD_WEB)
    add_subdirectory(wasm)
endif()

if(BUILD_TEST)
    add_compile_definitions(BUILD_TEST=1)
    add_executable(cpu_tests
            tests/cpu_tests.cpp
            core/CPU/address_modes.cpp
            core/CPU/bus.cpp
            core/CPU/cpu.cpp
            core/CPU/lookup.cpp
            core/CPU/opcodes.cpp
            tests/stub_ppu.cpp
            tests/stub_rom.cpp
    )
    add_test(NAME CpuTests COMMAND cpu_tests)

    add_executable(addressing_tests
            core/CPU/address_modes.cpp
            core/CPU/bus.cpp
            core/CPU/cpu.cpp
            core/CPU/lookup.cpp
            core/CPU/opcodes.cpp
            tests/stub_ppu.cpp
            tests/stub_rom.cpp
            tests/addressing_tests.cpp
    )
    add_test(NAME AddressingTests COMMAND addressing_tests)
endif()