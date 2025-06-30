#include <ctime>
#include <emscripten.h>
#include "../core/emu.h"

Emulator emulator;

constexpr int FRAMEBUFFER_WIDTH = 256;
constexpr int FRAMEBUFFER_HEIGHT = 240;

uint8_t framebuffer[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * 4]; // RGBA

extern "C" {

    // Initialize the emulator (called from JS)
    EMSCRIPTEN_KEEPALIVE
    void initEmu() {
        //emulator.reset();
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    // Run one frame of the emulator (called from JS)
    EMSCRIPTEN_KEEPALIVE
    void runFrame() {
        static uint8_t color = 0;
        color += 1;
        for (int i = 0; i < FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * 4; i += 4) {
            framebuffer[i + 0] = color;      // R
            framebuffer[i + 1] = 0;          // G
            framebuffer[i + 2] = 0;          // B
            framebuffer[i + 3] = 255;        // A
        }
    }


    // Get pointer to framebuffer (for JS to blit)
    EMSCRIPTEN_KEEPALIVE
    uint8_t* getFramebufferPtr() {
        //return emulator.getFrameBuffer();
        return framebuffer;
    }

}