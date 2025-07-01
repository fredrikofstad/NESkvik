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
    uint32_t initEmu() {
        //emulator.reset();
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        static uint32_t result[2] = {FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT};
        return reinterpret_cast<uint32_t>(result);
    }

    // Run one frame of the emulator (called from JS)
    EMSCRIPTEN_KEEPALIVE
    void runFrame() {
        for (int i = 0; i < FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * 4; i += 4) {
            uint8_t gray = std::rand() % 256;   // random brightness 0–255
            framebuffer[i + 0] = gray;          // R
            framebuffer[i + 1] = gray;          // G
            framebuffer[i + 2] = gray;          // B
            framebuffer[i + 3] = 255;           // A
        }
    }


    // Get pointer to framebuffer (for JS to blit)
    EMSCRIPTEN_KEEPALIVE
    uint8_t* getFramebufferPtr() {
        //return emulator.getFrameBuffer();
        return framebuffer;
    }

}