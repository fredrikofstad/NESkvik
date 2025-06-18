cmake_minimum_required(VERSION 3.31)
project(NESkvik)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/thirdparty)

set(SDL2_PATH "${CMAKE_SOURCE_DIR}/thirdparty/SDL2/")


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

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
        src/CPU/lookup.cpp
        src/PPU/ppu.cpp
        src/PPU/ppu.h
        src/ROM/rom.cpp
        src/ROM/rom.h
        src/Mapper/mapper.cpp
        src/Mapper/mapper.h
        src/Mapper/mapper_0.cpp
        src/Mapper/mapper_0.h)


add_library(imgui
        thirdparty/imgui/imgui.cpp
        thirdparty/imgui/imgui_draw.cpp
        thirdparty/imgui/imgui_widgets.cpp
        thirdparty/imgui/imgui_tables.cpp
        thirdparty/imgui/backends/imgui_impl_sdl2.cpp
        thirdparty/imgui/backends/imgui_impl_sdlrenderer2.cpp
)

target_include_directories(imgui PUBLIC
        thirdparty/imgui
        thirdparty/imgui/backends
)

target_link_libraries(
        ${PROJECT_NAME}
        imgui
        ${SDL2_LIBRARY}
)
