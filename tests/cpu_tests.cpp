#include "../core/CPU/cpu.h"
#include "../core/CPU/bus.h"
#include <cstdio>

int main() {
    Bus bus;

    // enable test mode
    bus.enableTestMode(true);
    printf("test mode enabled\n");

    // load Klaus binary
    bus.loadTestProgram("../../tests/bin/6502_functional_test.bin");

    // sanity print
    printf("testbin loaded, first opcode at 0x0400 = %02X\n", bus.ram[0x400]);

    // reset
    bus.reset();

    // run mode
    bus.setRunMode(RunMode::Running);

    // safety counter
    uint64_t tick_count = 0;
    const uint64_t max_ticks = 50'000'000;

    while (true) {
        bus.tick();
        tick_count++;

        // after reset, current_opcode is only valid once cycles==0
        if (bus.cpu.cycles == 0 && bus.cpu.current_opcode == 0x00) {
            printf("\nBRK detected at PC=%04X\n", bus.cpu.pc);
            printf("A=%02X X=%02X Y=%02X SP=%02X STATUS=%02X\n",
                   bus.cpu.a, bus.cpu.x, bus.cpu.y, bus.cpu.sp, bus.cpu.status);

            // typical Klaus success code: store to 0x0002
            printf("Final RAM[0x02] = %02X\n", bus.ram[0x02]);
            if (bus.ram[0x02] == 0) {
                printf("Klaus functional test PASSED!\n");
            } else {
                printf("Klaus functional test FAILED!\n");
            }
            break;
        }

        if (tick_count >= max_ticks) {
            printf("Timeout — gave up after %llu ticks.\n", max_ticks);
            break;
        }
    }

    return 0;
}
