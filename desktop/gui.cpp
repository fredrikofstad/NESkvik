//
// NESkvik - Fredrik Ofstad
//

#include "gui.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>

#include "imgui_internal.h"

#define WIDTH 256
#define HEIGHT 240


Gui::Gui(SDL_Window* window, SDL_Renderer* renderer, Emulator* emu)
    : m_renderer(renderer),
      m_framebuffer(std::make_unique<Framebuffer>(renderer, WIDTH, HEIGHT)),
      m_emu(emu),
      m_screen(*m_framebuffer),
      m_controls(&m_screen.scaleRef(), m_emu)


{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Enable docking
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
}

Gui::~Gui() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Gui::beginFrame() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void Gui::endFrame() {
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);
}

void Gui::render() {
    renderDockspace();

    ImGui::SetNextWindowDockID(ImGui::GetID("MainDockSpace"), ImGuiCond_FirstUseEver);
    if (m_showControls)
        m_controls.render();

    ImGui::SetNextWindowDockID(ImGui::GetID("MainDockSpace"), ImGuiCond_FirstUseEver);
    if (m_showScreen)
        m_screen.render();



    ImGui::SetNextWindowDockID(ImGui::GetID("MainDockSpace"), ImGuiCond_FirstUseEver);
}

void Gui::renderDockspace() {
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    const ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGui::SetNextWindowViewport(vp->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("DockRoot", nullptr, flags);
    ImGui::PopStyleVar(2);

    ImGuiID dockspaceID = ImGui::GetID("DockSpace");

    static bool firstTime = true;
    if (firstTime) {
        firstTime = false;
        ImGui::DockBuilderRemoveNode(dockspaceID); // clear any previous
        ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspaceID, vp->WorkSize);

        ImGuiID dock_main_id = dockspaceID;
        ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);

        ImGui::DockBuilderDockWindow("Screen", dock_main_id);
        ImGui::DockBuilderDockWindow("Controls", dock_id_right);

        ImGui::DockBuilderFinish(dockspaceID);

        ImGuiDockNode* screenNode = ImGui::DockBuilderGetNode(dock_main_id);
        if (screenNode) {
            screenNode->LocalFlags |= ImGuiDockNodeFlags_HiddenTabBar;
            screenNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
        }
    }

    renderMenuBar();
    ImGui::DockSpace(dockspaceID, ImVec2(0, 0), ImGuiDockNodeFlags_None);
    ImGui::End();
}

void Gui::renderMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                // TODO: implement exit logic if needed
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Panels")) {
            ImGui::MenuItem("Controls", nullptr, &m_showControls);
            ImGui::MenuItem("Screen", nullptr, &m_showScreen);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            ImGui::Text("nesKvik v0.1");
            ImGui::Text("An NES emulator by Fredrik Ofstad");
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}


void Gui::updateFrameBuffer() {
    m_framebuffer->update();
}


// Test


void Gui::updateFrameBufferTestPattern(float time) {
    constexpr int width = 256;
    constexpr int height = 240;

    auto& pixels = m_framebuffer->pixels();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            uint8_t r = static_cast<uint8_t>((x * 255) / (width - 1));
            uint8_t g = static_cast<uint8_t>(((y + static_cast<int>(time * 60)) % height) * 255 / (height - 1));
            uint8_t b = static_cast<uint8_t>(128 + 127 * std::sin(time + x * 0.05));
            uint8_t a = 255;

            pixels[y * width + x] = (a << 24) | (b << 16) | (g << 8) | r;
        }
    }

    m_framebuffer->update();
}