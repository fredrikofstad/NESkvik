//
// NESkvik - Fredrik Ofstad
//

#pragma once
#include "CPU/bus.h"

class Emulator {
public:
    Emulator() {
        cpu.AttachBus(&bus);
        bus.cpu = cpu;
    }

    void reset() { bus.reset(); }
    void step() { bus.tick(); }
    void runFrame(int cycles) {
        for (int i = 0; i < cycles; ++i) step();
    }

    // read-only getters for GUI
    uint16_t pc()   const { return cpu.pc; }
    uint8_t  a()    const { return cpu.a;  }
    uint8_t  x()    const { return cpu.x;  }
    uint8_t  y()    const { return cpu.y;  }
    uint32_t cycles()const{ return bus.cyclesCounter; }

    Bus  bus;
    CPU& cpu = bus.cpu;       // alias
};