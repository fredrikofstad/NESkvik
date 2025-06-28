add_executable(neskvik_web
        main.cpp
)

target_link_libraries(neskvik_web NESkvik)

set_target_properties(neskvik_web PROPERTIES
        OUTPUT_NAME "neskvik"
)

# pass these to *linker* (not compile)
set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} \
    -s MODULARIZE=1 \
    -s EXPORT_ES6=1 \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s EXPORTED_FUNCTIONS=['_initEmu','_runFrame','_getFramebufferPtr'] \
    -s EXPORTED_RUNTIME_METHODS=['cwrap','getValue','HEAPU8']"
)