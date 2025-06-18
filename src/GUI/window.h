//
// NESkvik - Fredrik Ofstad
//

#pragma once
#include <SDL.h>
#include <string>

class Window {
public:
    Window(const std::string& title, int w, int h);
    ~Window();
    bool isOpen() const { return m_running; }
    void pollEvents();
    SDL_Renderer* renderer() { return m_renderer; }
    SDL_Window* nativeWindow() const { return m_window; }
    void clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    void present();
    bool shouldQuit() const { return m_quit; }

private:
    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    bool          m_running  = false;
    bool          m_quit     = false;
};