//
// NESkvik - Fredrik Ofstad
//

#include <string>


class Emulator;

class ControlsPanel {
public:
    ControlsPanel(float* scalePtr, Emulator* emulator, const std::string& title = "Controls")
        : m_scale(scalePtr), m_title(title), m_emulator(emulator) {}

    void render();

private:
    float* m_scale = nullptr;
    std::string m_title;
    Emulator* m_emulator;
};
