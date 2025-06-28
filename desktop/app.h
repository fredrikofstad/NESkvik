//
// NESkvik - Fredrik Ofstad
//

#pragma once
#include "../core/emu.h"
#include "window.h"
#include "gui.h"

class App {
public:
    int run();                  // returns exit-code
private:
    Window m_window{"nesKvik", 800, 600};
    Emulator  m_emulator;
    Gui    m_gui{ m_window.nativeWindow(), m_window.renderer(), &m_emulator };
};