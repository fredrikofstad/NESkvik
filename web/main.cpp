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
        //emulator.runFrame(1);
        for (int i = 0; i < FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * 4; ++i) {
            framebuffer[i] = static_cast<uint8_t>(std::rand() % 256);
        }
    }

    // Get pointer to framebuffer (for JS to blit)
    EMSCRIPTEN_KEEPALIVE
    uint8_t* getFramebufferPtr() {
        //return emulator.getFrameBuffer();
        return framebuffer;
    }

}