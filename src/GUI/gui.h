//
// NESkvik - Fredrik Ofstad
//

#pragma once
#include <SDL.h>

#include "framebuffer.h"
#include "Panels/screen_panel.h"
#include "../types.h"
#include "Panels/control_panel.h"

class Emulator;

class Gui {
public:
    Gui(SDL_Window* window, SDL_Renderer* renderer, Emulator* emu);
    ~Gui();
    void beginFrame();          // sets up a new ImGui frame
    void endFrame();            // renders and presents ImGui draw data
    void render();

    void updateFrameBuffer();
    void updateFrameBufferTestPattern(float time);

private:
    SDL_Renderer* m_renderer;
    unique<Framebuffer> m_framebuffer;
    Emulator* m_emu;

    //panels
    ScreenPanel m_screen;
    ControlsPanel m_controls;

    // conditional panels
    bool m_showControls = true;
    bool m_showScreen = true;

    void renderDockspace();
    void renderMenuBar();
};
