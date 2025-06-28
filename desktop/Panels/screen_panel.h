//
// NESkvik - Fredrik Ofstad
//

#pragma once

#include <algorithm>
#include <string>

class Framebuffer;

class ScreenPanel {
public:
    ScreenPanel(Framebuffer& framebuffer, const std::string& title = "Screen");
    void setScale(float scale) {
        m_scale = std::clamp(scale, 0.1f, 3.0f);
    }
    float getScale() const { return m_scale; }
    float& scaleRef() { return m_scale; }

    void render();

private:
    Framebuffer& m_framebuffer;
    std::string m_title;
    float m_scale = 1.0f;
};
