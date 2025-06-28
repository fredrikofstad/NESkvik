//
// NESkvik - Fredrik Ofstad
//

#include "app.h"

int App::run() {

    while (!m_window.shouldQuit()) {
        m_window.pollEvents();

        m_emulator.runFrame(20);

        m_gui.beginFrame();
        m_gui.updateFrameBuffer();
        m_gui.render();

        m_window.clear(30, 30, 30);
        m_gui.endFrame();
        m_window.present();

    }

    return 0;
}