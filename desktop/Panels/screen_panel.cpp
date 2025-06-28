//
// NESkvik - Fredrik Ofstad
//

#include "screen_panel.h"
#include "../framebuffer.h"

#include <imgui.h>
#include <SDL.h>

ScreenPanel::ScreenPanel(Framebuffer& framebuffer, const std::string& title)
    : m_framebuffer(framebuffer), m_title(title) {}

void ScreenPanel::render() {
    ImGui::Begin(m_title.c_str());

    m_framebuffer.update();

    SDL_Texture* tex = m_framebuffer.getTexture();
    if (tex) {
        float fbWidth = static_cast<float>(m_framebuffer.width());
        float fbHeight = static_cast<float>(m_framebuffer.height());

        // Get the available panel size
        ImVec2 avail = ImGui::GetContentRegionAvail();

        // Calculate max scale that fits in panel
        float fitScaleX = avail.x / fbWidth;
        float fitScaleY = avail.y / fbHeight;
        float fitScale = std::min(fitScaleX, fitScaleY);

        // Apply user-defined scaling on top of the fitted size
        float finalScale = fitScale * m_scale;

        ImVec2 imageSize(fbWidth * finalScale, fbHeight * finalScale);


        ImGui::Image(reinterpret_cast<ImTextureID>(tex), imageSize);
    } else {
        ImGui::Text("Framebuffer not ready.");
    }

    ImGui::End();
}