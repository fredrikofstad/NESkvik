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
        src/CPU/addressing_modes.cpp
        src/CPU/addressing_modes.h)
