#include <functional>

#include "..//core/CPU/bus.h"
#include "..//core/CPU/cpu.h"

int tests_run = 0;
int tests_passed = 0;

void run_test(const char* name,
              std::function<void(Bus&)> setup,
              uint8_t expected_a,
              std::function<void(Bus&)> after_reset = nullptr)
{
    tests_run++;

    printf("========== %s ==========\n", name);

    Bus bus;
    bus.enableTestMode(true);
    bus.cpu.trace = true;

    // clear RAM
    bus.ram.fill(0);

    // initial memory/program setup
    setup(bus);

    // patch reset vector to start at 0x8000
    bus.ram[0xFFFC] = 0x00;
    bus.ram[0xFFFD] = 0x80;

    // perform reset
    bus.reset();

    // post-reset register initialization (like setting A/X/Y)
    if (after_reset)
        after_reset(bus);

    uint64_t tick_count = 0;
    const uint64_t max_ticks = 50;

    while (tick_count < max_ticks)
    {
        bus.cpu.clock();
        tick_count++;

        if (bus.cpu.cycles == 0)
        {
            // if BRK is hit and not at initial PC
            if (bus.cpu.current_opcode == 0x00 && bus.cpu.pc != 0x8000)
            {
                printf("\nBRK detected at PC=%04X\n", bus.cpu.pc);
                printf("Final A = %02X (expected %02X)\n", bus.cpu.a, expected_a);

                if (bus.cpu.a == expected_a)
                {
                    printf("[ OK ]\n");
                    tests_passed++;
                }
                else
                {
                    printf("[ FAIL ]\n");
                }
                printf("===========================================\n\n");
                return;
            }
        }
    }
    printf("[ TIMEOUT ]\n");
    printf("===========================================\n\n");
}


int main()
{
    run_test("LDA absolute",
        [](Bus& bus)
        {
            bus.ram[0x1234] = 0x42;
            bus.ram[0x8000] = 0xAD;  // LDA $1234
            bus.ram[0x8001] = 0x34;
            bus.ram[0x8002] = 0x12;
            bus.ram[0x8003] = 0x00;  // BRK
        },
        0x42
    );

    run_test("ADC absolute",
        [](Bus& bus)
        {
            bus.ram[0x1234] = 0x10;
            bus.ram[0x8000] = 0x6D;  // ADC $1234
            bus.ram[0x8001] = 0x34;
            bus.ram[0x8002] = 0x12;
            bus.ram[0x8003] = 0x00;  // BRK
        },
        0x20,
        [](Bus& bus) { bus.cpu.a = 0x10; }  // after reset
    );

    run_test("LDA immediate",
    [](Bus& bus)
    {
        bus.ram[0x8000] = 0xA9;  // LDA #$42
        bus.ram[0x8001] = 0x42;
        bus.ram[0x8002] = 0x00;  // BRK
    },
    0x42
    );

    run_test("LDA zero page",
    [](Bus& bus)
    {
        bus.ram[0x0010] = 0x55;
        bus.ram[0x8000] = 0xA5;  // LDA $10
        bus.ram[0x8001] = 0x10;
        bus.ram[0x8002] = 0x00;  // BRK
    },
    0x55
    );

    run_test("LDA zero page,X",
    [](Bus& bus)
    {
        bus.ram[0x0014] = 0x77;
        bus.ram[0x8000] = 0xB5;
        bus.ram[0x8001] = 0x10;
        bus.ram[0x8002] = 0x00;
    },
    0x77,
    [](Bus& bus)
    {
        bus.cpu.x = 0x04;
    }
    );

    run_test("LDA absolute,X",
    [](Bus& bus)
    {
        bus.ram[0x1238] = 0x55;                // memory with data
        bus.ram[0x8000] = 0xBD;                // LDA $1234,X
        bus.ram[0x8001] = 0x34;                // low byte
        bus.ram[0x8002] = 0x12;                // high byte
        bus.ram[0x8003] = 0x00;                // BRK
    },
    0x55,
    [](Bus& bus)
    {
        bus.cpu.x = 0x04;                      // add offset so effective address = $1234 + 4 = $1238
    }
    );

    run_test("LDA absolute,Y",
    [](Bus& bus)
    {
        bus.ram[0x2005] = 0x99;                // put test data
        bus.ram[0x8000] = 0xB9;                // LDA $2000,Y
        bus.ram[0x8001] = 0x00;
        bus.ram[0x8002] = 0x20;
        bus.ram[0x8003] = 0x00;                // BRK
    },
    0x99,
    [](Bus& bus)
    {
        bus.cpu.y = 0x05;                      // effective address = $2000 + 5 = $2005
    }
    );

    run_test("LDA immediate",
    [](Bus& bus)
    {
        bus.ram[0x8000] = 0xA9;  // LDA #$56
        bus.ram[0x8001] = 0x56;
        bus.ram[0x8002] = 0x00;  // BRK
    },
    0x56
    );

    run_test("LDA (indirect,X)",
    [](Bus& bus)
    {
        // put pointer table in zero page
        bus.ram[0x0004] = 0x00;   // low byte of target address
        bus.ram[0x0005] = 0x20;   // high byte of target address

        // put actual data
        bus.ram[0x2000] = 0xAB;

        // code
        bus.ram[0x8000] = 0xA1;   // LDA ($02,X)
        bus.ram[0x8001] = 0x02;
        bus.ram[0x8002] = 0x00;   // BRK
    },
    0xAB,
    [](Bus& bus)
    {
        bus.cpu.x = 0x02;   // so zero-page address becomes 0x0004
    }
    );

    run_test("LDA (indirect),Y",
    [](Bus& bus)
    {
        // pointer in zero-page
        bus.ram[0x0002] = 0x00;  // low byte
        bus.ram[0x0003] = 0x20;  // high byte

        // target data
        bus.ram[0x2004] = 0xCD;

        // code
        bus.ram[0x8000] = 0xB1;   // LDA ($02),Y
        bus.ram[0x8001] = 0x02;
        bus.ram[0x8002] = 0x00;   // BRK
    },
    0xCD,
    [](Bus& bus)
    {
        bus.cpu.y = 0x04;   // base 0x2000 + 0x04 = 0x2004
    }
    );

    run_test("LDA absolute,X",
    [](Bus& bus)
    {
        // data to load
        bus.ram[0x2010] = 0xAB;

        // code
        bus.ram[0x8000] = 0xBD;  // LDA $2000,X
        bus.ram[0x8001] = 0x00;
        bus.ram[0x8002] = 0x20;
        bus.ram[0x8003] = 0x00;  // BRK
    },
    0xAB,
    [](Bus& bus)
    {
        bus.cpu.x = 0x10;
    }
    );

    run_test("LDA absolute,Y",
    [](Bus& bus)
    {
        // place data at effective address
        bus.ram[0x3030] = 0xCD;

        // code
        bus.ram[0x8000] = 0xB9;  // LDA $3000,Y
        bus.ram[0x8001] = 0x00;
        bus.ram[0x8002] = 0x30;
        bus.ram[0x8003] = 0x00;  // BRK
    },
    0xCD,
    [](Bus& bus)
    {
        bus.cpu.y = 0x30;
    }
    );

    run_test("LDA absolute,X",
    [](Bus& bus)
    {
        // place data at effective address
        bus.ram[0x3050] = 0xEF;

        // code
        bus.ram[0x8000] = 0xBD;  // LDA $3000,X
        bus.ram[0x8001] = 0x00;
        bus.ram[0x8002] = 0x30;
        bus.ram[0x8003] = 0x00;  // BRK
    },
    0xEF,
    [](Bus& bus)
    {
        bus.cpu.x = 0x50;
    }
    );

    run_test("ADC immediate",
    [](Bus& bus)
    {
        bus.ram[0x8000] = 0x69;  // ADC #$22
        bus.ram[0x8001] = 0x22;
        bus.ram[0x8002] = 0x00;  // BRK
    },
    0x32,   // 0x10 + 0x22
    [](Bus& bus)
    {
        bus.cpu.a = 0x10;
    }
);

run_test("ADC zero page",
    [](Bus& bus)
    {
        bus.ram[0x0010] = 0x05;
        bus.ram[0x8000] = 0x65;  // ADC $10
        bus.ram[0x8001] = 0x10;
        bus.ram[0x8002] = 0x00;
    },
    0x15,
    [](Bus& bus)
    {
        bus.cpu.a = 0x10;
    }
);

run_test("ADC zero page,X",
    [](Bus& bus)
    {
        bus.ram[0x0014] = 0x03;
        bus.ram[0x8000] = 0x75;  // ADC $10,X
        bus.ram[0x8001] = 0x10;
        bus.ram[0x8002] = 0x00;
    },
    0x13,
    [](Bus& bus)
    {
        bus.cpu.a = 0x10;
        bus.cpu.x = 0x04;
    }
);

run_test("ADC absolute",
    [](Bus& bus)
    {
        bus.ram[0x1234] = 0x20;
        bus.ram[0x8000] = 0x6D;
        bus.ram[0x8001] = 0x34;
        bus.ram[0x8002] = 0x12;
        bus.ram[0x8003] = 0x00;
    },
    0x30,
    [](Bus& bus)
    {
        bus.cpu.a = 0x10;
    }
);

run_test("ADC absolute,X",
    [](Bus& bus)
    {
        bus.ram[0x1244] = 0x15;
        bus.ram[0x8000] = 0x7D;
        bus.ram[0x8001] = 0x34;
        bus.ram[0x8002] = 0x12;
        bus.ram[0x8003] = 0x00;
    },
    0x25,
    [](Bus& bus)
    {
        bus.cpu.a = 0x10;
        bus.cpu.x = 0x10;
    }
);

run_test("ADC absolute,Y",
    [](Bus& bus)
    {
        bus.ram[0x1244] = 0x05;
        bus.ram[0x8000] = 0x79;
        bus.ram[0x8001] = 0x34;
        bus.ram[0x8002] = 0x12;
        bus.ram[0x8003] = 0x00;
    },
    0x15,
    [](Bus& bus)
    {
        bus.cpu.a = 0x10;
        bus.cpu.y = 0x10;
    }
);

run_test("ADC (indirect,X)",
    [](Bus& bus)
    {
        // pointer in zero page
        bus.ram[0x0004] = 0x00;
        bus.ram[0x0005] = 0x20;
        bus.ram[0x2000] = 0x11;

        bus.ram[0x8000] = 0x61;  // ADC ($02,X)
        bus.ram[0x8001] = 0x02;
        bus.ram[0x8002] = 0x00;
    },
    0x21,
    [](Bus& bus)
    {
        bus.cpu.a = 0x10;
        bus.cpu.x = 0x02;
    }
);

run_test("ADC (indirect),Y",
    [](Bus& bus)
    {
        bus.ram[0x0002] = 0x00;
        bus.ram[0x0003] = 0x20;
        bus.ram[0x2005] = 0x04;

        bus.ram[0x8000] = 0x71;
        bus.ram[0x8001] = 0x02;
        bus.ram[0x8002] = 0x00;
    },
    0x14,
    [](Bus& bus)
    {
        bus.cpu.a = 0x10;
        bus.cpu.y = 0x05;
    }
    );

    run_test("BNE forward branch taken",
    [](Bus& bus)
    {
        bus.ram[0x8000] = 0xD0;  // BNE +2
        bus.ram[0x8001] = 0x02;
        bus.ram[0x8003] = 0x00;  // BRK at target

        bus.cpu.setFlag(flags::Zero, false);  // Zero flag clear, so BNE will branch
    },
    0x00 // A not modified, so expected 0
);

    run_test("BNE forward branch not taken",
    [](Bus& bus)
    {
        bus.ram[0x8000] = 0xD0;  // BNE +2
        bus.ram[0x8001] = 0x02;
        bus.ram[0x8002] = 0x00;  // BRK next

        bus.cpu.setFlag(flags::Zero, true);  // Zero flag set, so BNE will not branch
    },
    0x00
);

    run_test("INX implied",
    [](Bus& bus)
    {
        bus.ram[0x8000] = 0xE8;  // INX
        bus.ram[0x8001] = 0x00;  // BRK
    },
    0x00,  // A stays zero
    [](Bus& bus)
    {
        bus.cpu.x = 0x10;
    }
);

    run_test("JMP indirect",
    [](Bus& bus)
    {
        bus.ram[0x3000] = 0x00;  // low byte of target
        bus.ram[0x3001] = 0x90;  // high byte of target
        bus.ram[0x9000] = 0x00;  // BRK at 0x9000

        bus.ram[0x8000] = 0x6C;  // JMP ($3000)
        bus.ram[0x8001] = 0x00;
        bus.ram[0x8002] = 0x30;
    },
    0x00  // A unaffected
);

    run_test("LDX zero page,Y",
    [](Bus& bus)
    {
        bus.ram[0x0014] = 0x44; // $10 + Y=4
        bus.ram[0x8000] = 0xB6; // LDX $10,Y
        bus.ram[0x8001] = 0x10;
        bus.ram[0x8002] = 0x00; // BRK
    },
    0x00, // A unchanged
    [](Bus& bus)
    {
        bus.cpu.y = 0x04;
    }
);

    printf("===========================================\n");
    printf("SUMMARY: %d/%d tests passed\n", tests_passed, tests_run);
    printf("===========================================\n");

    return 0;
}
