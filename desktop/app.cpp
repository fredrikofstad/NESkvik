//
// NESkvik - Fredrik Ofstad
//

#include "app.h"

#include <iostream>

int App::run() {

    // Load ROM once at startup before entering main loop
    std::string romPath = R"(C:\Dev\C\NESkvik\tests\roms\nestest.nes)"; // Adjust this to your ROM path
    auto rom = std::make_shared<ROM>(romPath);

    if (!rom->isImageValid()) {
        // Handle invalid ROM error, maybe print and exit
        std::cerr << "Failed to load ROM from " << romPath << std::endl;
        return -1;
    }

    m_emulator.attachRom(rom);

    // Optionally reset the emulator now that ROM is attached
    m_emulator.reset();

    while (!m_window.shouldQuit()) {
        m_window.pollEvents();

        // Run emulation frames; tune timing as needed
        m_emulator.runFrame();

        m_gui.beginFrame();

        // Update framebuffer from PPU's rendered data
        m_gui.updateFrameBuffer();

        m_gui.render();

        m_window.clear(30, 30, 30);
        m_gui.endFrame();
        m_window.present();
    }

    return 0;
}
