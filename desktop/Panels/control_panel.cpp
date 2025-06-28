//
// NESkvik - Fredrik Ofstad
//

#include "control_panel.h"
#include "../../core/emu.h"

#include <cstdio> // for snprintf
#include <imgui.h>

void ControlsPanel::render() {
    ImGui::Begin(m_title.c_str());
    ImGui::SliderFloat("Scale", m_scale, 0.1f, 3.0f, "%.2fx");

    // --- Run control buttons ---
    if (ImGui::Button("Step"))
        m_emulator->bus.setRunMode(RunMode::Step);

    ImGui::SameLine();
    if (ImGui::Button("Run"))
        m_emulator->bus.setRunMode(RunMode::Running);

    ImGui::SameLine();
    if (ImGui::Button("Pause"))
        m_emulator->bus.setRunMode(RunMode::Paused);

    if (ImGui::Button("Reset"))
        m_emulator->reset();

    ImGui::SameLine();
    if (ImGui::Button("IRQ"))
        m_emulator->bus.cpu.irq();

    ImGui::SameLine();
    if (ImGui::Button("NMI"))
        m_emulator->bus.cpu.nmi();

    ImGui::Separator();

    // --- CPU Registers ---
    const CPU& c = m_emulator->bus.cpu;
    ImGui::Text("PC : $%04X", c.pc);
    ImGui::Text("A  : $%02X", c.a);
    ImGui::Text("X  : $%02X", c.x);
    ImGui::Text("Y  : $%02X", c.y);
    ImGui::Text("SP : $%02X", c.sp);
    ImGui::Text("STATUS: N=%d V=%d -=%d B=%d D=%d I=%d Z=%d C=%d",
                (c.status>>7)&1,(c.status>>6)&1,(c.status>>5)&1,(c.status>>4)&1,
                (c.status>>3)&1,(c.status>>2)&1,(c.status>>1)&1,(c.status>>0)&1);

    ImGui::Separator();

    // --- Simple RAM hex dump (page $0000 and $8000) ---
    auto dump = [&](uint16_t base, int rows, int cols)
    {
        char linebuf[512];
        for (int r=0; r<rows; ++r)
        {
            int offset = snprintf(linebuf, sizeof(linebuf), "$%04X:", base + r*cols);
            for (int c=0; c<cols; ++c)
            {
                uint8_t v = m_emulator->bus.read(base + r*cols + c, true);
                offset += snprintf(linebuf + offset, sizeof(linebuf) - offset, " %02X", v);
            }
            ImGui::TextUnformatted(linebuf);
        }
    };

    dump(0x0000, 8, 16);   // zero-page
    ImGui::Dummy(ImVec2(0,5));
    dump(0x8000, 8, 16);   // program area

    ImGui::End();
}
