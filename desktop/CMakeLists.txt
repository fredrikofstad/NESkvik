# SDL2 configuration
if(MSVC)
    message(STATUS "Using SDL2 from thirdparty/SDL2/vc for MSVC")
    set(SDL2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/SDL2/include/SDL2")
    set(SDL2_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/thirdparty/SDL2/vc")
    set(SDL2_DLL_PATH "${CMAKE_SOURCE_DIR}/thirdparty/SDL2/vc")
    link_directories(${SDL2_LIBRARY_DIR})
    set(SDL2_LIBRARIES SDL2 SDL2main)
else()
    message(STATUS "Using system or default SDL2")
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/thirdparty)
    find_package(SDL2 REQUIRED)
    set(SDL2_LIBRARIES ${SDL2_LIBRARY})
    set(SDL2_DLL_PATH "${CMAKE_SOURCE_DIR}/thirdparty/SDL2/bin")
endif()

# imgui as separate static lib
add_library(imgui
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_sdl2.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_sdlrenderer2.cpp
    ${CMAKE_SOURCE_DIR}/desktop/Panels/screen_panel.cpp
    ${CMAKE_SOURCE_DIR}/desktop/Panels/screen_panel.h
    ${CMAKE_SOURCE_DIR}/desktop/Panels/control_panel.cpp
    ${CMAKE_SOURCE_DIR}/desktop/Panels/control_panel.h
)

target_include_directories(imgui SYSTEM PUBLIC
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/backends
    ${SDL2_INCLUDE_DIR}
)

target_link_libraries(imgui PRIVATE ${SDL2_LIBRARIES})

# NESkvikApp executable
add_executable(NESkvikApp
    ${CMAKE_SOURCE_DIR}/desktop/window.cpp
    ${CMAKE_SOURCE_DIR}/desktop/window.h
    ${CMAKE_SOURCE_DIR}/desktop/app.cpp
    ${CMAKE_SOURCE_DIR}/desktop/app.h
    ${CMAKE_SOURCE_DIR}/desktop/gui.cpp
    ${CMAKE_SOURCE_DIR}/desktop/gui.h
    ${CMAKE_SOURCE_DIR}/desktop/framebuffer.cpp
    ${CMAKE_SOURCE_DIR}/desktop/framebuffer.h
    ${CMAKE_SOURCE_DIR}/desktop/main.cpp
)

set_target_properties(NESkvikApp PROPERTIES OUTPUT_NAME "neskvik")

target_include_directories(NESkvikApp SYSTEM PRIVATE
    ${SDL2_INCLUDE_DIR}
)

target_link_libraries(NESkvikApp
    PRIVATE NESkvik imgui ${SDL2_LIBRARIES}
)

# copy SDL2.dll after build
add_custom_command(TARGET NESkvikApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL2_DLL_PATH}/SDL2.dll"
    $<TARGET_FILE_DIR:NESkvikApp>
)
