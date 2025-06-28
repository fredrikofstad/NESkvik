cmake_minimum_required(VERSION 3.31)
project(NESkvik)

set(CMAKE_CXX_STANDARD 20)

# optionally build web or desktop
option(BUILD_WEB "Build the web version via Emscripten" OFF)
option(BUILD_DESKTOP "Build the native desktop GUI" OFF)


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
)



if(BUILD_DESKTOP)

    # Third-party dependencies

    # imgui as separate static lib
    add_library(imgui
            thirdparty/imgui/imgui.cpp
            thirdparty/imgui/imgui_draw.cpp
            thirdparty/imgui/imgui_widgets.cpp
            thirdparty/imgui/imgui_tables.cpp
            thirdparty/imgui/backends/imgui_impl_sdl2.cpp
            thirdparty/imgui/backends/imgui_impl_sdlrenderer2.cpp
            desktop/Panels/screen_panel.cpp
            desktop/Panels/screen_panel.h
            desktop/Panels/control_panel.cpp
            desktop/Panels/control_panel.h
    )

    target_include_directories(imgui SYSTEM PUBLIC
            thirdparty/imgui
            thirdparty/imgui/backends
    )

    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/thirdparty)
    set(SDL2_PATH "${CMAKE_SOURCE_DIR}/thirdparty/SDL2/")
    set(SDL2_DLL_PATH "${CMAKE_SOURCE_DIR}/thirdparty/SDL2/bin")

    find_package(SDL2 REQUIRED)


    include_directories(${SDL2_INCLUDE_DIR})

    add_executable(NESkvikApp
            desktop/window.cpp
            desktop/window.h
            desktop/app.cpp
            desktop/app.h
            desktop/gui.cpp
            desktop/gui.h
            desktop/framebuffer.cpp
            desktop/framebuffer.h
            desktop/main.cpp
    )

    set_target_properties(NESkvikApp PROPERTIES OUTPUT_NAME "neskvik")

    target_include_directories(NESkvikApp SYSTEM PRIVATE
            ${SDL2_INCLUDE_DIR}
    )

    target_link_libraries(NESkvikApp
            PRIVATE NESkvik imgui ${SDL2_LIBRARY}
    )

    # need the dll in same folder as executable
    add_custom_command(TARGET NESkvikApp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DLL_PATH}/SDL2.dll"
            $<TARGET_FILE_DIR:NESkvikApp>
    )
endif()

# Web version
if(BUILD_WEB)
    add_subdirectory(web)
endif()