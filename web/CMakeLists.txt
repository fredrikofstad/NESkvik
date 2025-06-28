add_executable(neskvik_web
        main.cpp
)

target_link_libraries(neskvik_web
        NESkvik
)

# Force the output to HTML to trigger JS+WASM generation
set_target_properties(neskvik_web PROPERTIES
        SUFFIX ".html"
        OUTPUT_NAME "neskvik"
)

# Emscripten-specific flags
target_compile_options(neskvik_web PRIVATE
        "-sWASM=1"
        "-sMODULARIZE=1"
        "-sEXPORT_ES6=1"
        "-sALLOW_MEMORY_GROWTH=1"
)

# Add exported functions and runtime methods
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -sEXPORTED_FUNCTIONS=['_initEmu','_runFrame','_getFramebufferPtr'] -sEXPORTED_RUNTIME_METHODS=['cwrap','getValue','HEAPU8']")

