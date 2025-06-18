//
// NESkvik - Fredrik Ofstad
//

#pragma once
#include "emu.h"
#include "GUI/window.h"
#include "GUI/gui.h"

class App {
public:
    int run();                  // returns exit-code
private:
    Window m_window{"nesKvik", 800, 600};
    Emulator  m_emulator;
    Gui    m_gui{ m_window.nativeWindow(), m_window.renderer(), &m_emulator };
};